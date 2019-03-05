#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iomanip>
#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"
#include "media/h264_wrap.h"

using namespace std;

#define BUFSIZE_DEFAULT 200000
//定义包头长度，RTMP_MAX_HEADER_SIZE=18
#define RTMP_HEAD_SIZE (sizeof(RTMPPacket) + RTMP_MAX_HEADER_SIZE)
//存储Nal单元数据的buffer大小
#define BUFFER_SIZE 32768
//搜寻Nal单元时的一些标志
#define GOT_A_NAL_CROSS_BUFFER BUFFER_SIZE + 1
#define GOT_A_NAL_INCLUDE_A_BUFFER BUFFER_SIZE + 2
#define NO_MORE_BUFFER_TO_READ BUFFER_SIZE + 3

typedef struct
{
    int pos;
    unsigned char *buff;
    int buffSize;
    int alreadSeen;
} BUFFER_INFO;

typedef struct
{
    unsigned char *sps;
    int spsLen;
    unsigned char *pps;
    int ppsLen;
} RTMPMetaData;

class RtmpH264
{
  public:
    RtmpH264() : rtmp(NULL), bufferInfo(NULL), h264Wrap(NULL)
    {
    }
    ~RtmpH264()
    {
        if (rtmp)
        {
            if (RTMP_IsConnected(rtmp))
            {
                RTMP_Close(rtmp);
            }
            RTMP_Free(rtmp);
        }
    }
    bool Init();
    bool Connect(const string &url);
    bool SendH264Packet(unsigned char *data, int size, bool isKeyFrame, unsigned int timeStamp);
    bool PublishH264(const string &fileaname);

  protected:
    int SendPacket(unsigned int packetType, unsigned char *data, unsigned int size, unsigned int timestamp);
    bool SendVideoSpsPps(const RTMPMetaData &mediaData);
    bool SetPPS(RTMPMetaData &metaData, unsigned char *pps, int ppsLen);
    bool SetSPS(RTMPMetaData &metaData, unsigned char *sps, int spsLen);

    RTMPMetaData metaData;
    RTMP *rtmp;
    BUFFER_INFO *bufferInfo;
    H264Wrap *h264Wrap;
};

bool RtmpH264::SendVideoSpsPps(const RTMPMetaData &mediaData)
{
    int i = 0;
    if (!mediaData.pps || mediaData.ppsLen <= 0)
    {
        LOG_ERR("pps is invalid");
        return false;
    }
    if (!mediaData.sps || mediaData.spsLen <= 0)
    {
        LOG_ERR("sps is invalid");
        return false;
    }
    RTMPPacket *packet = (RTMPPacket *)new unsigned char[RTMP_HEAD_SIZE + 1024];
    if (!packet)
    {
        LOG_ERR("create RTMP Packet failed");
        return false;
    }

    memset(packet, 0, RTMP_HEAD_SIZE + 1024);
    packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
    unsigned char *body = (unsigned char *)packet->m_body;

    i = 0;
    body[i++] = 0x17;
    body[i++] = 0x00;

    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;

    /*AVCDecoderConfigurationRecord*/
    body[i++] = 0x01;
    body[i++] = metaData.sps[1];
    body[i++] = metaData.sps[2];
    body[i++] = metaData.sps[3];
    body[i++] = 0xff;

    /*sps*/
    body[i++] = 0xe1;
    body[i++] = (metaData.spsLen >> 8) & 0xff;
    body[i++] = metaData.spsLen & 0xff;
    memcpy(&body[i], metaData.sps, metaData.spsLen);
    i += metaData.spsLen;

    /*pps*/
    body[i++] = 0x01;
    body[i++] = (metaData.ppsLen >> 8) & 0xff;
    body[i++] = (metaData.ppsLen) & 0xff;
    memcpy(&body[i], metaData.pps, metaData.ppsLen);
    i += metaData.ppsLen;

    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize = i;
    packet->m_nChannel = 0x04;
    packet->m_nTimeStamp = 0;
    packet->m_hasAbsTimestamp = 0;
    packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    packet->m_nInfoField2 = rtmp->m_stream_id;

    int ret = RTMP_SendPacket(rtmp, packet, TRUE);

    delete[] packet;

    return ret ? true : false;
}

bool RtmpH264::SetPPS(RTMPMetaData &metaData, unsigned char *pps, int ppsLen)
{
    if (metaData.pps)
    {
        delete[] metaData.pps;
        metaData.pps = NULL;
    }
    if (NULL == (metaData.pps = new unsigned char[ppsLen]))
    {
        return false;
    }

    metaData.ppsLen = ppsLen;
    memcpy(metaData.pps, pps, ppsLen);
    return true;
}
bool RtmpH264::SetSPS(RTMPMetaData &metaData, unsigned char *sps, int spsLen)
{
    if (metaData.sps)
    {
        delete[] metaData.sps;
        metaData.sps = NULL;
    }
    if (NULL == (metaData.sps = new unsigned char[spsLen]))
    {
        return false;
    }

    metaData.spsLen = spsLen;
    memcpy(metaData.sps, sps, spsLen);
    return true;
}

bool RtmpH264::Init()
{
    memset(&metaData, 0, sizeof(metaData));
    if (NULL == (h264Wrap = new H264Wrap()))
    {
        return false;
    }
    if (!(bufferInfo = new BUFFER_INFO()))
    {
        return false;
    }
    if (!(bufferInfo->buff = new unsigned char[BUFSIZE_DEFAULT]))
    {
        delete bufferInfo;
        bufferInfo = NULL;
        return false;
    }

    bufferInfo->pos = 0;
    bufferInfo->buffSize = BUFSIZE_DEFAULT;
    bufferInfo->alreadSeen = 0;

    rtmp = RTMP_Alloc();
    if (!rtmp)
    {
        RTMP_Log(RTMP_LOGERROR, "alloc RTMP failed");
        return false;
    }
    RTMP_Init(rtmp);
    return true;
}

bool RtmpH264::Connect(const string &url)
{
    if (!RTMP_SetupURL(rtmp, (char *)(url.c_str())))
    {
        RTMP_Log(RTMP_LOGERROR, "SetupURL(%s) failed", url.c_str());
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

bool RtmpH264::PublishH264(const string &filename)
{
    //READ sps
    ifstream inFile(filename, ios::in | ios::binary);
    if (!inFile)
    {
        LOG_ERR("open file(%s) failed", filename);
        return false;
    }

    NaluUnit naluUnit;
    int nextFramePos;

    if (!inFile.read(reinterpret_cast<char *>(&bufferInfo->buff[0]) + bufferInfo->alreadSeen, bufferInfo->buffSize - bufferInfo->pos))
    {
        LOG_ERR("read file failed");
        return false;
    }
    bufferInfo->alreadSeen += inFile.gcount();

    if (0 > (nextFramePos = h264Wrap->GetNaluFromBuffer(naluUnit, bufferInfo->buff + bufferInfo->pos, bufferInfo->alreadSeen)))
    {
        inFile.close();
        LOG_ERR("cannot find sps");
        return false;
    }
    bufferInfo->pos += nextFramePos;
    LOG_INFO("sps nalu:%d", nextFramePos);

    if (!SetSPS(metaData, naluUnit.data, naluUnit.size))
    {
        inFile.close();
        LOG_ERR("set sps failed");
        return false;
    }
    //READ pps
    if (0 > (nextFramePos = h264Wrap->GetNaluFromBuffer(naluUnit, bufferInfo->buff + bufferInfo->pos, bufferInfo->alreadSeen)))
    {
        inFile.close();
        LOG_ERR("cannot find pps");
        return false;
    }
    bufferInfo->pos += nextFramePos;

    LOG_INFO("pps nalu:%d", nextFramePos);
    if (!SetPPS(metaData, naluUnit.data, naluUnit.size))
    {
        inFile.close();
        LOG_ERR("set pps failed");
        return false;
    }

    int width = 0, height = 0, fps = 0;
    if (false == h264Wrap->DecodeSPS(metaData.sps, metaData.spsLen, width, height, fps))
    {
        inFile.close();
        LOG_ERR("parse sps failed");
        return false;
    }

    if (!fps)
    {
        fps = 25;
    }

    unsigned int now = 0;
    unsigned int lastUpdate = 0;
    unsigned int tick = 0;
    unsigned int tickGap = 1000 / fps;
    bool isKeyFrame = false;

    int readCount;

    while (inFile.read(reinterpret_cast<char *>(&bufferInfo->buff[0]) + bufferInfo->alreadSeen, bufferInfo->buffSize - bufferInfo->alreadSeen))
    {
        readCount = inFile.gcount();
        bufferInfo->alreadSeen += readCount;

        while ((nextFramePos = h264Wrap->GetNaluFromBuffer(naluUnit, bufferInfo->buff + bufferInfo->pos, bufferInfo->alreadSeen - bufferInfo->pos)) > 0)
        {
            LOG_DEBUG("nalu: startcode:%d,type:%d, data:%p, dataSize:%d,nextFramePos:%d",
                      naluUnit.startCode, naluUnit.type, naluUnit.data, naluUnit.size, nextFramePos);
            bufferInfo->pos += nextFramePos;
            //                pos          alreadSeen      bufSize
            // |  #############| ############| #############|
            if (naluUnit.type == NALU_TYPE_SPS || naluUnit.type == NALU_TYPE_PPS)
            {
                bufferInfo->pos += nextFramePos;
                continue;
            }

            isKeyFrame = (naluUnit.type == NALU_TYPE_IDR) ? true : false;
            if (!SendH264Packet(naluUnit.data, naluUnit.size, isKeyFrame, tick))
            {
                break;
            }

            tick += tickGap;
            now = RTMP_GetTime();

            if ((tickGap + lastUpdate) > now)
            {
                LOG_DEBUG("sleep %d ms", tickGap + lastUpdate - now);
                msleep(tickGap + lastUpdate - now);
            }
            lastUpdate = now;
        }
        if (bufferInfo->pos > 0)
        {
            memmove(bufferInfo->buff, bufferInfo->buff + bufferInfo->pos, bufferInfo->alreadSeen - bufferInfo->pos); //memcpy会有内存重叠问题
            bufferInfo->alreadSeen -= bufferInfo->pos;
            bufferInfo->pos = 0;
        }
        else
        {
            bufferInfo->pos = 0;
            bufferInfo->alreadSeen = 0;
        }
    }

    return true;
}

bool RtmpH264::SendH264Packet(unsigned char *data, int size, bool isKeyFrame, unsigned int timeStamp)
{
    int i = 0;
    if (!data || size < 11)
    {
        return false;
    }
    unsigned char *body = new unsigned char[size + 9];
    if (!body)
    {
        LOG_ERR("new body failed");
        return false;
    }
    memset(body, 0, size + 9);
    if (isKeyFrame)
    {
        body[i++] = 0x17; // 1:Iframe  7:AVC
        body[i++] = 0x01; // AVC NALU
        body[i++] = 0x00;
        body[i++] = 0x00;
        body[i++] = 0x00;

        // NALU size
        body[i++] = size >> 24 & 0xff;
        body[i++] = size >> 16 & 0xff;
        body[i++] = size >> 8 & 0xff;
        body[i++] = size & 0xff;
        // NALU data
        memcpy(&body[i], data, size);
        if (false == SendVideoSpsPps(metaData))
        {
            LOG_ERR("send sps pps failed");
        }
    }
    else
    {
        body[i++] = 0x27; // 2:Pframe  7:AVC
        body[i++] = 0x01; // AVC NALU
        body[i++] = 0x00;
        body[i++] = 0x00;
        body[i++] = 0x00;

        // NALU size
        body[i++] = size >> 24 & 0xff;
        body[i++] = size >> 16 & 0xff;
        body[i++] = size >> 8 & 0xff;
        body[i++] = size & 0xff;
        // NALU data
        memcpy(&body[i], data, size);
    }
    int ret = SendPacket(RTMP_PACKET_TYPE_VIDEO, body, i + size, timeStamp);
    delete[] body;
    return ret ? true : false;
}

int RtmpH264::SendPacket(unsigned int packetType, unsigned char *data, unsigned int size, unsigned int timestamp)
{
    RTMPPacket *packet = (RTMPPacket *)malloc(RTMP_HEAD_SIZE + size);
    if (!packet)
    {
        LOG_ERR("malloc RTMPPacket failed:%d", RTMP_HEAD_SIZE + size);
        return -1;
    }
    memset(packet, 0, RTMP_HEAD_SIZE);

    packet->m_body = (char *)(packet) + RTMP_HEAD_SIZE;
    packet->m_nBodySize = size;
    memcpy(packet->m_body, data, size);
    packet->m_hasAbsTimestamp = 0;
    packet->m_packetType = packetType;
    packet->m_nInfoField2 = rtmp->m_stream_id;
    packet->m_nChannel = 0x04;

    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    if (RTMP_PACKET_TYPE_AUDIO == packetType && size != 4)
    {
        packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    }
    packet->m_nTimeStamp = timestamp;
    /*发送*/
    int nRet = 0;
    if (RTMP_IsConnected(rtmp))
    {
        nRet = RTMP_SendPacket(rtmp, packet, TRUE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
    }
    /*释放内存*/
    free(packet);
    return nRet;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return -1;
    string filename(argv[1]);
    string url = "rtmp://127.0.0.1:1935/live/test264";
    int ret = -1;
    RtmpH264 *rtmpH264 = new RtmpH264();
    if (!rtmpH264)
    {
        LOG_ERR("new RtmpH264 failed");
        goto do_exit;
    };
    if (!rtmpH264->Init())
    {
        LOG_ERR("rtmpH264 init failed");
        goto do_exit;
    }

    if (!rtmpH264->Connect(url))
    {
        LOG_ERR("connect %s failed", url);
        goto do_exit;
    }

    if (!rtmpH264->PublishH264(filename))
    {
        LOG_ERR("publish %s failed", filename.c_str());
        goto do_exit;
    }

    LOG_INFO("publish %s suc", filename.c_str());
    ret = 0;
do_exit:
    if (rtmpH264)
    {
        delete rtmpH264;
    }
    return ret;
}
