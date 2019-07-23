#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>  // cv::Canny()
#include <iostream>

using namespace cv;
using std::cout; using std::cerr; using std::endl;


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void openCamera();
    void readFrame();
    void closeCamera();
    void takingPictures();

private:
    Ui::Widget *ui;

    QTimer *timer;
    QImage *image;
    VideoCapture cap;
    Mat frame;
};

#endif // WIDGET_H
