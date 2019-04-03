#ifndef RTMP_SERVER_HH
#define RTMP_SERVER_HH

#include <string>
#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

#define DEFAUKT_PACKAGE_SIZE (64 * 1024)

typedef ssize_t (*GetDataCB)(const void *&buf, ssize_t max, void *arg);

typedef enum
{
    MEDIA_TYPE_H264 = 0,
    MEDIA_TYPE_FLV,
} MEDIA_TYPE_T;

typedef struct
{
    MEDIA_TYPE_T type;
    GetDataCB cb;
} INPUT_STREAM;

class RTMPServer
{
  public:
    RTMPServer();
    ~RTMPServer();
    bool Init();
    bool Connect(std::string url);

    bool PublishStream(INPUT_STREAM *stream, void *arg);
  protected:
    bool PublishFLV(GetDataCB cb, unsigned int packageSize, void *arg);

  private:
    RTMP *rtmp;
};

#endif