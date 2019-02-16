#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QImage>
#include <QTimer>

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

namespace Ui {
class Camera;
}

class Camera : public QWidget
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    ~Camera();

private slots:
    void OpenCamera();
    void ReadFrame();
    void CloseCamera();
    void TakingPictures();

private:
    Ui::Camera *ui;
    QTimer *timer;
    QImage *image;
    CvCapture *cam;
    IplImage *frame;

};

#endif // CAMERA_H
