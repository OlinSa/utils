#include <iostream>
#include "videoplayer.h"

using namespace std;

VideoPlayer::VideoPlayer()
{
    url = "";
    outBuffer = NULL;
    frameRGB = NULL;
    codecCtx = NULL;
    formatCtx = NULL;
    state = PLAYER_STATE_UNINIT;
    videoColor = VIDEO_NONE;
}

VideoPlayer::VideoPlayer(std::string url)
{
    this->url = url;
    outBuffer = NULL;
    frameRGB = NULL;
    codecCtx = NULL;
    formatCtx = NULL;
    state = PLAYER_STATE_UNINIT;
    videoColor = VIDEO_NONE;
}

VideoPlayer::~VideoPlayer()
{
    if(outBuffer) {
        av_free(outBuffer);
    }
    if(frameRGB) {
        av_free(frameRGB);
    }
    if(codecCtx) {
        avcodec_close(codecCtx);
    }
    if(formatCtx) {
        avformat_close_input(&formatCtx);
    }
}

bool VideoPlayer::setURL(std::string url)
{
    this->url = url;
    return true;
}
std::string VideoPlayer::getURL()
{
    return url;
}

bool VideoPlayer::Init()
{
    if(NULL == (formatCtx = avformat_alloc_context())) {
        cout<<"alloc format context failed"<<endl;
        return false;
    }

    AVDictionary *options=NULL;
//    av_dict_set(&options, "rtsp_transport", "tcp", 0);

    if(avformat_open_input(&formatCtx, url.c_str(), NULL, NULL) != 0) {
        cout<<"open "<<url<<" failed"<<endl;
        return false;
    }
    if(avformat_find_stream_info(formatCtx, NULL) < 0){
        cout<<"could't not find stream information"<<endl;
        return false;
    }

    videoIndex = av_find_best_stream(formatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if(videoIndex < 0) {
        cout<<"did not find video stream"<<endl;
        return false;
    }
    av_dump_format(formatCtx,0,url.c_str(),0);

    origin_par = formatCtx->streams[videoIndex]->codecpar;
    //find decoder
    codec = avcodec_find_decoder(origin_par->codec_id);
    if(NULL == codec) {
        cout<<"could not open codec:"<<codecCtx->codec_id<<endl;
        return false;
    }
    codecCtx = avcodec_alloc_context3(codec);
    if(!codecCtx) {
        cout<<"Can't allcate decoder context"<<endl;
        return false;
    }

    if(avcodec_parameters_to_context(codecCtx, origin_par)) {
        cout<<"can't copy decoder context"<<endl;
        return false;
    }

    if(avcodec_open2(codecCtx, codec, NULL) < 0){
        cout<<"can't open decoder"<<endl;
        return false;
    }

    frame = av_frame_alloc();
    frameRGB = av_frame_alloc();

    if(!frame || !frameRGB) {
        cout<<"frame alloc failed"<<endl;
        return false;
    }
    imgConvertCtx = sws_getContext(codecCtx->width, codecCtx->height,
                                   codecCtx->pix_fmt, codecCtx->width, codecCtx->height,
                                   AV_PIX_FMT_RGB32, SWS_BICUBIC, nullptr, NULL, NULL);

    size_t numBytes = av_image_get_buffer_size(codecCtx->pix_fmt, codecCtx->width, codecCtx->height, 16);
    cout<<"picture size:"<<numBytes<<endl;
    outBuffer = (uint8_t*)av_malloc(numBytes*sizeof(size_t));
    av_image_fill_arrays(frameRGB->data, frameRGB->linesize, outBuffer, AV_PIX_FMT_RGB32, codecCtx->width, codecCtx->height, 1);

    packet = av_packet_alloc();
    av_init_packet(packet);
    state = PLAYER_STATE_READY;
    return true;

}
void VideoPlayer::run()
{
    int ret;
    int gotPicture = -1;
    while(state == PLAYER_STATE_RUNING) {
        if(av_read_frame(formatCtx, packet) < 0) {
            cout<<"read frame finial"<<endl;
            break;
        }
        if(packet->stream_index == videoIndex) {
            ret = avcodec_decode_video2(codecCtx, frame, &gotPicture, packet);
            if(ret < 0) {
                cout<<"decodec error"<<endl;
                break;
            }
            if(gotPicture) {
                sws_scale(imgConvertCtx, frame->data, frame->linesize, 0, codecCtx->height, frameRGB->data,
                          frameRGB->linesize);
                QImage image((uchar *)outBuffer ,codecCtx->width,codecCtx->height,QImage::Format_RGB32);
                emit sig_GetOneFrame(image);
                int color;
                for(int i = 0; i < codecCtx->width; i++) {
                    for(int j = 0; j < codecCtx->height; j++) {
                        QRgb rgb = image.pixel(i,j);

                        switch(videoColor) {
                        case VIDEO_RED:{
                            color = qRed(rgb);
                            image.setPixel(i, j, qRgb(color,0,0));
                        }break;
                        case VIDEO_BLUE: {
                            color = qBlue(rgb);
                            image.setPixel(i, j, qRgb(0,0,color));
                        }break;
                        case VIDEO_GREEN: {
                            color = qGreen(rgb);
                            image.setPixel(i, j, qRgb(0,color,0));
                        }break;
                        default:{
                            break;
                        }
                        }
                    }
                }
                emit sig_GetRFrame(image);
            }
        }
        av_packet_unref(packet); //释放资源,否则内存会一直上升
        msleep(0.02);
    }
    cout<<"thread exit"<<endl;
}

bool VideoPlayer::Start()
{
    state = PLAYER_STATE_RUNING;
    this->start();
    return true;
}
bool VideoPlayer::Stop()
{
    state = PLAYER_STATE_STOP;
    return true;
}

void VideoPlayer::SetVideoColor(VIDEO_COLOR_T color)
{
   this->videoColor = color;
   return;
}
