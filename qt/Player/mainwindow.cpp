#include <QPainter>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new VideoPlayer("rtsp://192.168.124.128:8554/test.264");
    connect(player, SIGNAL(sig_GetOneFrame(QImage)), this, SLOT(slotGetOneFrame(QImage)));
    connect(player, SIGNAL(sig_GetRFrame(QImage)), this, SLOT(slotGetRFrame(QImage)));
    connect(ui->Open_red, &QAction::triggered, this, &MainWindow::slotOpenRed);
    connect(ui->Close_Red, &QAction::triggered, this, &MainWindow::slotCloseRed);
    if(player->Init()) {
        player->start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::white);
    painter.drawRect(0,0,this->width(),this->height());

    if(image.size().width() <= 0) return;

    QImage img = image.scaled(this->size(), Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x, y), img);

    if(openRed) {
        QWidget *redVideo = new QWidget(this);
        redVideo->resize(this->width()/3, this->height() /3 );
        painter.setBrush(Qt::white);
        painter.drawRect(0,0, redVideo->width(), redVideo->height());
        QImage rImg = rImage.scaled(redVideo->size(), Qt::KeepAspectRatio);

        int rx = redVideo->width() - rImg.width();
        int ry = redVideo->height() - rImg.height();

        rx /= 2;
        ry /= 2;

        painter.drawImage(QPoint(rx, ry), rImg);

    }
}

QImage Indentificate(QImage image)
{
    QSize size = image.size();
    QImage binaryImage(size, QImage::Format_RGB32);
    int width = image.width(), height = image.height();
    int threshold = 200;
    for (int i = 0; i<width; i++)
        for (int j = 0; j < height; j++)
        {
            QRgb pixel = image.pixel(i, j);
            int r = (int)(qRed(pixel) * 0.3);
            int g = (int)(qGreen(pixel) * 0.59);
            int b = (int)(qBlue(pixel) * 0.11);
            int rgb = r + g + b;        //先把图像灰度化，转化为灰度图像
            if (rgb > threshold)        //然后按某一阀值进行二值化
                rgb = 255;
            else
                rgb = 0;
            QRgb newPixel = qRgb(rgb, rgb, rgb);
            binaryImage.setPixel(i, j, newPixel);
        }
    return binaryImage;
}


void MainWindow::slotGetOneFrame(QImage image)
{
    this->image = image;
    update();//call paintEvent
}
void MainWindow::slotGetRFrame(QImage image)
{
    rImage = image;
    update();
}
bool MainWindow::slotOpenRed()
{
    openRed = true;
    return openRed;
}
bool MainWindow::slotCloseRed()
{
    openRed = false;
    return openRed;
}
