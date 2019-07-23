#include <iostream>
#include <QFileDialog>
#include <unistd.h>
#include "pushflow.h"
#include "ui_pushflow.h"

using namespace std;

static double r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}

int avError(int errNum) {
    char buf[1024];
    //获取错误信息
    av_strerror(errNum, buf, sizeof(buf));
    cout << " failed! " << buf << endl;
    return -1;
}

PushFlow::PushFlow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PushFlow)
{
    ui->setupUi(this);
    ui->lineEditSrcURI->setText(QString::fromStdString("test.264"));
    ui->lineEditDstURI->setText(QString::fromStdString("rtmp://127.0.0.1:1935/live/test264"));

}

PushFlow::~PushFlow()
{
    delete ui;
}

void PushFlow::on_pushButton_clicked()
{
    int videoIndex;
    std::string inputURI = ui->lineEditSrcURI->text().toStdString();
    std::string outputURI = ui->lineEditDstURI->text().toStdString();
    cout<<"src:"<<inputURI<<endl;
    cout<<"dst:"<<outputURI<<endl;
    sleep(5);

    AVFormatContext *ictx = NULL;
    AVOutputFormat *ofmt = NULL;

    //打开文件，解封文件头
    int ret = avformat_open_input(&ictx, inputURI.c_str(), 0, NULL);
    if (ret < 0) {
        avError(ret);
        return;
    }
    cout << "avformat_open_input success!" << endl;
    ret = avformat_find_stream_info(ictx, 0);
    if (ret != 0) {
        avError(ret);
        return;
    }
    //打印视频视频信息
    //0打印所有  inUrl 打印时候显示，
    av_dump_format(ictx, 0, inputURI.c_str(), 0);

    //////////////////////////////////////////////////////////////////
    //                   输出流处理部分
    /////////////////////////////////////////////////////////////////
    AVFormatContext * octx = NULL;
    //如果是输入文件 flv可以不传，可以从文件中判断。如果是流则必须传
    //创建输出上下文
    ret = avformat_alloc_output_context2(&octx, NULL, "flv", outputURI.c_str());
    if (ret < 0) {
        avError(ret);
        return;
    }
    cout << "avformat_alloc_output_context2 success!" << endl;

    ofmt = octx->oformat;
    cout << "nb_streams  " << ictx->nb_streams << endl;
    int i;
    for (i = 0; i < ictx->nb_streams; i++) {

        //获取输入视频流
        AVStream *in_stream = ictx->streams[i];
        //为输出上下文添加音视频流（初始化一个音视频流容器）
        AVStream *out_stream = avformat_new_stream(octx, in_stream->codec->codec);
        if (!out_stream) {
            printf("未能成功添加音视频流\n");
            ret = AVERROR_UNKNOWN;
        }

        //将输入编解码器上下文信息 copy 给输出编解码器上下文
        ret = avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar);
        if (ret < 0) {
            cout<<"copy 编解码器上下文失败"<<endl;
        }
        out_stream->codecpar->codec_tag = 0;

        out_stream->codec->codec_tag = 0;
        //        if (octx->oformat->flags & AVFMT_GLOBALHEADER) {
        //            out_stream->codec->flags = out_stream->codec->flags | CODEC_FLAG_GLOBAL_HEADER;
        //        }
    }

    //输入流数据的数量循环
    for (i = 0; i < ictx->nb_streams; i++) {
        if (ictx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }

    av_dump_format(octx, 0, outputURI.c_str(), 1);

    //////////////////////////////////////////////////////////////////
    //                   准备推流
    /////////////////////////////////////////////////////////////////

    //打开IO
    ret = avio_open(&octx->pb, outputURI.c_str(), AVIO_FLAG_WRITE);
    if (ret < 0) {
        avError(ret);
    }

    //写入头部信息
    ret = avformat_write_header(octx, 0);
    if (ret < 0) {
        avError(ret);
    }
    cout << "avformat_write_header Success!" << endl;
    //推流每一帧数据
    //int64_t pts  [ pts*(num/den)  第几秒显示]
    //int64_t dts  解码时间 [P帧(相对于上一帧的变化) I帧(关键帧，完整的数据) B帧(上一帧和下一帧的变化)]  有了B帧压缩率更高。
    AVPacket pkt;
    //获取当前的时间戳  微妙
    long long start_time = av_gettime();
    long long frame_index = 0;
    while (1) {
        //输入输出视频流
        AVStream *in_stream, *out_stream;
        //获取解码前数据
        ret = av_read_frame(ictx, &pkt);
        if (ret < 0) {
            break;
        }

        /*
         * PTS（Presentation Time Stamp）显示播放时间
         * DTS（Decoding Time Stamp）解码时间
         */
        //没有显示时间（比如未解码的 H.264 ）
        if (pkt.pts == AV_NOPTS_VALUE) {
            //AVRational time_base：时基。通过该值可以把PTS，DTS转化为真正的时间。
            AVRational time_base1 = ictx->streams[videoIndex]->time_base;

            //计算两帧之间的时间
            /*
                r_frame_rate 基流帧速率
                av_q2d 转化为double类型
                */
            int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(ictx->streams[videoIndex]->r_frame_rate);

            //配置参数
            pkt.pts = (double)(frame_index*calc_duration) / (double)(av_q2d(time_base1)*AV_TIME_BASE);
            pkt.dts = pkt.pts;
            pkt.duration = (double)calc_duration / (double)(av_q2d(time_base1)*AV_TIME_BASE);
        }

        //延时
        if (pkt.stream_index == videoIndex) {
            AVRational time_base = ictx->streams[videoIndex]->time_base;
            AVRational time_base_q = { 1,AV_TIME_BASE };
            //计算视频播放时间
            int64_t pts_time = av_rescale_q(pkt.dts, time_base, time_base_q);
            //计算实际视频的播放时间
            int64_t now_time = av_gettime() - start_time;

            AVRational avr = ictx->streams[videoIndex]->time_base;
            cout << avr.num << " " << avr.den << "  "<<pkt.dts <<"  "<<pkt.pts<<"   "<< pts_time <<endl;
            if (pts_time > now_time) {
                //播放时间同步
                cout<<"pts:"<<pts_time<<" now:"<<now_time<<endl;
                av_usleep((unsigned int)(pts_time - now_time));
            }
        }

        in_stream = ictx->streams[pkt.stream_index];
        out_stream = octx->streams[pkt.stream_index];

        //计算延时后，重新指定时间戳
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,(AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        pkt.duration = (int)av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        //字节流的位置，-1 表示不知道字节流位置
        pkt.pos = -1;

        if (pkt.stream_index == videoIndex) {
            printf("Send %8d video frames to output URL\n", frame_index);
            frame_index++;
        }

        //向输出上下文发送（向地址推送）
        ret = av_interleaved_write_frame(octx, &pkt);

        if (ret < 0) {
            printf("发送数据包出错\n");
            break;
        }

        //释放
        av_free_packet(&pkt);
    }
    return;
}

void PushFlow::on_pushButtonSelectSrcURI_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("file"), "", "");
    if(!fileName.isNull()) {
        ui->lineEditSrcURI->setText(fileName);
    }
}

void PushFlow::on_pushButtonDstURI_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("file"), "", "");
    if(!fileName.isNull()) {
        ui->lineEditDstURI->setText(fileName);
    }
}
