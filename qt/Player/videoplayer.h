#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <SDL2/SDL.h>

#ifdef __cplusplus
}
#endif

#include <QThread>
#include <QImage>
#include <string>

typedef enum {
    VIDEO_NONE = 0,
    VIDEO_RED,
    VIDEO_GREEN,
    VIDEO_BLUE
}VIDEO_COLOR_T;

typedef enum {
    PLAYER_STATE_UNINIT = 0,
    PLAYER_STATE_READY,
    PLAYER_STATE_STOP,
    PLAYER_STATE_RUNING,

}PLAYER_STATE;

class VideoPlayer:public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayer();
    explicit VideoPlayer(std::string url);
    ~VideoPlayer();
    bool Init();
    bool Start();
    bool Stop();
    void run();

    void SetVideoColor(VIDEO_COLOR_T color);

signals:
    void sig_GetOneFrame(QImage image);
    void sig_GetRFrame(QImage image);

public:
    bool setURL(std::string url);
    std::string getURL();


protected:
    VIDEO_COLOR_T videoColor;
    std::string url;
    int videoIndex; //stream info index
private:

    AVFormatContext *formatCtx;
    AVCodecContext *codecCtx;
    AVCodecParameters *origin_par;
    AVCodec *codec;
    AVFrame *frame, *frameRGB;
    AVPacket *packet;
    uint8_t *outBuffer;

    struct SwsContext *imgConvertCtx;
    PLAYER_STATE state;
};

#endif // VIDEOPLAYER_H
