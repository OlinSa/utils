#ifndef PUSHFLOW_H
#define PUSHFLOW_H

//参考自:https://cloud.tencent.com/developer/article/1193989

#include <QDialog>

namespace Ui {
class PushFlow;
}

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/time.h>
#include <SDL2/SDL.h>

#ifdef __cplusplus
}
#endif

class PushFlow : public QDialog
{
    Q_OBJECT

public:
    explicit PushFlow(QWidget *parent = nullptr);
    ~PushFlow();
private slots:
    void on_pushButton_clicked();

    void on_pushButtonSelectSrcURI_clicked();

    void on_pushButtonDstURI_clicked();

private:
    Ui::PushFlow *ui;

    AVFormatContext *ictx;
};

#endif // PUSHFLOW_H
