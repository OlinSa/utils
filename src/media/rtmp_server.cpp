#include "rtmp_server.h"
#include "log.h"

RTMPServer::RTMPServer()
            :rtmp(NULL)
{

}
RTMPServer::~RTMPServer()
{

}
bool RTMPServer::Init()
{
    rtmp = RTMP_Alloc();
    if(!rtmp) {
        LOG_ERR("Alloc RTMP failed");
        return false;
    }
    RTMP_Init(rtmp);
    return true;
}
bool RTMPServer::Connect(std::string url)
{
    if(!RTMP_SetupURL(rtmp,(char*)(url.c_str()))) {
        LOG_ERR("Setup URL(%s) failed", url.c_str());
        return false;
    }
    if(!RTMP_ConnectStream(rtmp, 0)) {
        LOG_ERR("connect stream failed");
        return false;
    }
    return true;
}

// --------------------------------------------------------------------------------------
//     FLV Header(FLAG(3Byte) + VERSION(1BYTE) + STEAMINFO(1BYTE) + HEADERLENGTH(4BYTE))
// --------------------------------------------------------------------------------------
//     FLV Body
// --------------------------------------------------------------------------------------

bool RTMPServer::PublishFLV(GetDataCB cb，unsigned int packageSize, void *arg)
{
    RTMPPacket *package = new RTMPPacket();
    if(!packet) {
        LOG_ERR("new RTMP Pakcage failed");
        return false;
    }
    if(1 != RTMPPacket_Alloc(packet, packageSize)) {
        LOG_ERR("Alloc package failed:%d", packageSize);
        return false;
    }
    RTMPPacket_Reset(packet);

    packet->m_hasAbsTimestamp = 0;
    packet->m_nChannel = 0x04;
    packet->m_nInfoField2 = rtmp->m_stream_id;

    const void *buf = NULL;

    ssize_t size = cb(buf, packageSize, arg);

    //skip flv header(9Byte abd oerviudTag size(3Byte)
    if(size < 13) {
        LOG_ERR("Incomplete package, package size(%d)", size);
        return false;
    }

    uint32_t start_time = RTMP_GetTime();
    uint32_t now_time;
    uint32_t pre_frame_time = 0;
    uint32_t lasttime = 0;

    //flv 流信息
    uint32_t type = 0; //flv type
    uint32_t dataLength = 0;
    uint32_t timestamp = 0;
    uint32_t streamid = 0;
    uint32_t preTagSize = 0;

    bool bNextIsKey = false;

    do {
        now_time = RTMP_GetTime()- starttime;
        if (now_time < pre_frame_time && bNextIsKey) {
            if(pre_frame_time > lasttime){
                // LOG_INFO("TimeStamp:%u")
            }
        }
    }while(1);


    delete package;
}

bool RTMPServer::PublishStream(INPUT_STREAM *stream, void *arg)
{
    bool ret = false;
    switch (stream->type)
    {
        case MEDIA_TYPE_FLV:
            ret = PublishFLV(stream->cb, DEFAUKT_PACKAGE_SIZE, arg);
            break;
    
        default:
            LOG_ERR("unsupport MEDIA_TPYE:%d", stream->type);
    }
    return ret;
}