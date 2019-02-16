#include "camera.h"
#include "ui_camera.h"

Camera::Camera(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Camera)
{
    ui->setupUi(this);

    cam = NULL;

}

Camera::~Camera()
{
    delete ui;
}
