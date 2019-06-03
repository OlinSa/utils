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

class VideoPlayer:public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayer(std::string url);
    ~VideoPlayer();
    bool Init();
    void run();

signals:
    void sig_GetOneFrame(QImage image);
    void sig_GetRFrame(QImage image);

protected:
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

};

#endif // VIDEOPLAYER_H
