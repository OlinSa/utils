#ifndef FLV_IF_HH
#define FLV_IF_HH

#include <string>
#include <limits.h>

typedef struct
{
    std::string markding; //文件标示
    uint8_t version;
    uint32_t headerLength;
} FLV_HEADER_T;

typedef struct
{
    uint8_t type;        //0x08:audio 0x09:video 0x12:scripe data ...
    uint32_t datasize;   //tag data size
    uint32_t timeStamp;  //整数，单位是毫秒。对于脚本型的tag总是0 （CTS）
    uint8_t timeStampEx; //将时间戳扩展为4bytes，代表高8位
    uint32_t streamID;   // always 0
} FLV_TAG_HEADER;

// 音频编码类型(4bit) | 采样率(2bit) | 精度(1bit) | 类型（1bit) | 音频数据 ....|
typedef struct
{
    uint8_t soundFormat : 4;
    uint8_t soundRate : 2; //5.5KHZ 11KHZ 22KHZ 44KHZ
    uint8_t soundSize : 1; //8bit 16bit
    uint8_t soundType : 1; //单声道|立体声
    uint8_t aacPacketType; //if soundFormat == 10,   0:AAC sequence header 1:ACC raw

    char *body; //tagdata
} FLV_AUDIO_TAG_DATA;

// 帧类型(4bit) | 视频编码类型(4bit) | 视频数据 ...|
typedef struct
{
    uint8_t frameType : 4; //1:key Frame 2:inte frame ...
    uint8_t codecID : 4;
    uint8_t avPacketType;    //if codecID == 7
    int32_t compositionTIme; //if codecID == 7

    char *body;
} FLV_VIDEO_TAG_DATA;

// AMF1("onMetaData") | AMF2("widht, height...")
typedef struct
{
    uint8_t type;
    char *body;
} FLV_SCRIPT_TAG_DATA;

typedef struct
{
    union {
        FLV_AUDIO_TAG_DATA tagAudioData;
        FLV_VIDEO_TAG_DATA videTagData;
        FLV_SCRIPT_TAG_DATA scriptTagData;
    } tagData;
} FLV_TAG_DATA;
typedef struct
{
    FLV_TAG_HEADER tagHeader;
    FLV_TAG_DATA tagData;
} FLV_TAG;
typedef struct
{
    uint32_t previousTagSize; //前一个tag size
    FLV_TAG tag;

} FLV_BODY_T;

class Flv
{
  public:
    Flv();
    ~Flv();
    int setFilename(std::string filename);
    int printFlvInfo();

  protected:
};

#endif