#include <iostream>
#include <stdio.h>
#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

using namespace std;

//AMF采用大端模式
#define HTON16(x) ((x >> 8 & 0xff) | (x << 8 & 0xff00))
#define HTON24(x) ((x >> 16 & 0xff) | (x << 16 & 0xff0000) | (x & 0xff00))
#define HTON32(x) ((x >> 24 & 0xff) | (x >> 8 & 0xff00) | \
                   (x << 8 & 0xff0000) | (x << 24 & 0xff000000))
#define HTONTIME(x) ((x >> 16 & 0xff) | (x << 16 & 0xff0000) | (x & 0xff00) | (x & 0xff000000))

/*read 1 byte*/
int ReadU8(uint32_t *u8, FILE *fp)
{
    if (fread(u8, 1, 1, fp) != 1)
        return 0;
    return 1;
}
/*read 2 byte*/
int ReadU16(uint32_t *u16, FILE *fp)
{
    if (fread(u16, 2, 1, fp) != 1)
        return 0;
    *u16 = HTON16(*u16);
    return 1;
}
/*read 3 byte*/
int ReadU24(uint32_t *u24, FILE *fp)
{
    if (fread(u24, 3, 1, fp) != 1)
        return 0;
    *u24 = HTON24(*u24);
    return 1;
}
/*read 4 byte*/
int ReadU32(uint32_t *u32, FILE *fp)
{
    if (fread(u32, 4, 1, fp) != 1)
        return 0;
    *u32 = HTON32(*u32);
    return 1;
}
/*read 1 byte,and loopback 1 byte at once*/
int PeekU8(uint32_t *u8, FILE *fp)
{
    if (fread(u8, 1, 1, fp) != 1)
        return 0;
    fseek(fp, -1, SEEK_CUR);
    return 1;
}
/*read 4 byte and convert to time format*/
int ReadTime(uint32_t *utime, FILE *fp)
{
    if (fread(utime, 4, 1, fp) != 1)
        return 0;
    *utime = HTONTIME(*utime);
    return 1;
}

class Rtmp
{
  public:
    Rtmp()
    {
        rtmp = NULL;
    }
    ~Rtmp()
    {
        if (rtmp)
        {
            RTMP_Close(rtmp);
            RTMP_Free(rtmp);
        }
    }
    bool init();
    void exit();
    bool connect(char *url);
    bool publishFlv(const char *filename);

  protected:
    RTMP *rtmp;
};

bool Rtmp::init()
{
    rtmp = RTMP_Alloc();
    if (!rtmp)
    {
        RTMP_Log(RTMP_LOGERROR, "alloc RTMP failed");
        return false;
    }
    RTMP_Init(rtmp);
    return true;
}

bool Rtmp::connect(char *url)
{
    if (!RTMP_SetupURL(rtmp, url))
    {
        RTMP_Log(RTMP_LOGERROR, "SetupURL(%s) failed", url);
        return false;
    }
    RTMP_EnableWrite(rtmp);

    if (!RTMP_Connect(rtmp, NULL))
    {
        RTMP_Log(RTMP_LOGERROR, "connect failed");
        return false;
    }

    if (!RTMP_ConnectStream(rtmp, 0))
    {
        RTMP_Log(RTMP_LOGERROR, "connect stream failed");
        return false;
    }
    return true;
}

// --------------------------------------------------------------------------------------
//     FLV Header(FLAG(3Byte) + VERSION(1BYTE) + STEAMINFO(1BYTE) + HEADERLENGTH(4BYTE))
// --------------------------------------------------------------------------------------
//     FLV Body
// --------------------------------------------------------------------------------------

bool Rtmp::publishFlv(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        RTMP_LogPrintf("open %s failed", filename);
        return false;
    }

    RTMPPacket *packet = new RTMPPacket();
    if (!packet)
    {
        RTMP_Log(RTMP_LOGERROR, "new RTMPPacket failed");
        return false;
    }
    RTMPPacket_Alloc(packet, 1024 * 64);
    RTMPPacket_Reset(packet);

    packet->m_hasAbsTimestamp = 0;
    packet->m_nChannel = 0x04;
    packet->m_nInfoField2 = rtmp->m_stream_id;

    RTMP_LogPrintf("start to send data\n");

    //skip flv header
    fseek(fp, 9, SEEK_SET);
    //skip previoudTag size
    fseek(fp, 4, SEEK_CUR);

    uint32_t start_time = RTMP_GetTime();
    uint32_t now_time;
    uint32_t pre_frame_time = 0;
    uint32_t lasttime = 0;
    bool bNextIsKey = false;

    //flv 流信息
    uint32_t type = 0; //flv type
    uint32_t dataLength = 0;
    uint32_t timestamp = 0;
    uint32_t streamid = 0;
    uint32_t preTagSize = 0;
    while (1)
    {
        now_time = RTMP_GetTime() - start_time;
        if (now_time < pre_frame_time && bNextIsKey)
        {
            //发送线程太快，等待
            if (pre_frame_time > lasttime)
            {
                RTMP_LogPrintf("TimeStamp:%u ms\n", pre_frame_time);
                lasttime = pre_frame_time;
            }
            sleep(1);
            continue;
        }

        //读取type
        if (!ReadU8(&type, fp))
        {
            break;
        }
        if (!ReadU24(&dataLength, fp))
        {
            break;
        }
        if (!ReadTime(&timestamp, fp))
        {
            break;
        }
        if (!ReadU24(&streamid, fp))
        {
            break;
        }

        if (type != 0x08 && type != 0x09)
        {
            //skip non_audio and non_video frame
            //skip next previousTagSizen(4Byte)
            fseek(fp, dataLength + 4, SEEK_CUR);
            continue;
        }
        if (fread(packet->m_body, 1, dataLength, fp) != dataLength)
        {
            RTMP_LogPrintf("read %u Byte failed", dataLength);
            break;
        }

        packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
        packet->m_nTimeStamp = timestamp;
        packet->m_packetType = type;
        packet->m_nBodySize = dataLength;

        if (!RTMP_IsConnected(rtmp))
        {
            RTMP_Log(RTMP_LOGERROR, "rtmp is not connect\n");
            break;
        }

        if (!RTMP_SendPacket(rtmp, packet, 0))
        {
            RTMP_Log(RTMP_LOGERROR, "send failed");
            break;
        }

        if (!ReadU32(&preTagSize, fp))
        {
            break;
        }

        //读取下一帧type
        if (!PeekU8(&type, fp))
        {
            break;
        }

        //新的一帧
        if (type == 0x09)
        {
            //skip tag  header
            if (fseek(fp, 11, SEEK_CUR) != 0)
            {
                break;
            }
            //tag data
            if (!PeekU8(&type, fp))
            {
                break;
            }
            if (type == 0x17)
            { //key frame + AVC
                bNextIsKey = 1;
            }
            else
            {
                bNextIsKey = 0;
            }
            fseek(fp, -11, SEEK_CUR);
        }
    }
    RTMP_LogPrintf("send data over");

    if (fp)
    {
        fclose(fp);
    }

    if (packet)
    {
        RTMPPacket_Free(packet);
        delete packet;
    }
    return true;
}

int main(int argc, char *argv[])
{
    const char *filename = "test.flv";
    const char *url = "rtmp://127.0.0.1:1935/live/test264";
    Rtmp *rtmp = new Rtmp();
    if (!rtmp)
    {
        cout << "new Rtmp failed" << endl;
        return -1;
    }
    if (!rtmp->init())
    {
        cout << "rtmp init failed" << endl;
        goto do_exit;
    }
    if (!rtmp->connect((char *)url))
    {
        cout << "connect " << url << " failed" << endl;
        goto do_exit;
    }
    if (!rtmp->publishFlv(filename))
    {
        cout << "pushlish " << filename << " failed" << endl;
        goto do_exit;
    }
    cout << "publish " << filename << " success" << endl;
do_exit:
    delete rtmp;
    return 0;
}