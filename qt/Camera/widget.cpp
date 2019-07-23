#include "widget.h"
#include "ui_widget.h"
#include <opencv2/videoio/videoio_c.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    image = new QImage();

    connect(timer, SIGNAL(timeout()), this, SLOT(readFrame()));
    connect(ui->pushButtonOpen, SIGNAL(clicked()), this, SLOT(openCamera()));
    connect(ui->pushButtonPhoto, SIGNAL(clicked()), this, SLOT(takingPictures()));
    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(closeCamera()));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::openCamera()
{
    cap.open(0);
    if(!cap.isOpened()) {
        cerr<<"ERROR:Can't init camera"<<endl;
        return;
    }

    timer->start(33);

}
void Widget::readFrame()
{
    cap >> frame;
    QImage image((const uchar*)frame.data, cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT), QImage::Format_RGB888);
    image = image.scaled(ui->label->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(QPixmap::fromImage(image));

    //图像适配label
//    ui->label->resize(ui->label->pixmap()->size());
}
void Widget::closeCamera()
{
    timer->stop();
}
void Widget::takingPictures()
{
    cap >> frame;
    QImage image((const uchar*)frame.data, cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT), QImage::Format_RGB888);
    image = image.scaled(ui->label_2->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label_2->setPixmap(QPixmap::fromImage(image));
//    ui->label_2->resize(ui->label->pixmap()->size());
}
