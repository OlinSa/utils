#include <QPainter>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置窗口为model模式
    configureDialog = new ConfigureDialog();
    configureDialog->setWindowModality(Qt::ApplicationModal);


    pushFlow = new PushFlow();
    pushFlow->setWindowModality(Qt::ApplicationModal);

    player = new VideoPlayer();
    connect(player, SIGNAL(sig_GetOneFrame(QImage)), this, SLOT(slotGetOneFrame(QImage)));
    connect(player, SIGNAL(sig_GetRFrame(QImage)), this, SLOT(slotGetRFrame(QImage)));
    connect(ui->actionRed, &QAction::triggered, this, &MainWindow::slotClickRed);
    connect(ui->actionGreen, &QAction::triggered, this, &MainWindow::slotClickGreen);
    connect(ui->actionBlue, &QAction::triggered, this, &MainWindow::slotClickBlue);
    connect(ui->actionClose, &QAction::triggered, this, &MainWindow::slotClickClose);

    connect(ui->actionStreamPull, &QAction::triggered, this, &MainWindow::slotPull);
    connect(ui->actionStreamPush, &QAction::triggered, this, &MainWindow::slotPush);

    videoColor = VIDEO_NONE;
}

MainWindow::~MainWindow()
{
    delete configureDialog;
    delete pushFlow;
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

    if(videoColor != VIDEO_NONE) {
        QWidget *video = new QWidget(this);
        video->resize(this->width()/3, this->height() /3 );
        painter.setBrush(Qt::white);
        painter.drawRect(0,0, video->width(), video->height());
        QImage rImg = rImage.scaled(video->size(), Qt::KeepAspectRatio);

        int rx = video->width() - rImg.width();
        int ry = video->height() - rImg.height();

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

bool MainWindow::slotClickRed()
{
    videoColor = VIDEO_RED;
    player->SetVideoColor(videoColor);
    return true;
}
bool MainWindow::slotClickGreen()
{
    videoColor = VIDEO_GREEN;
    player->SetVideoColor(videoColor);
    return true;
}
bool MainWindow::slotClickBlue()
{
    videoColor = VIDEO_BLUE;
    player->SetVideoColor(videoColor);
    return true;
}
bool MainWindow::slotClickClose()
{
    videoColor = VIDEO_NONE;
    player->SetVideoColor(videoColor);
    return true;
}

bool MainWindow::slotPull()
{
    configureDialog->show();
    return  true;
}

bool MainWindow::slotPush()
{
    pushFlow->show();
    return  true;
}

void MainWindow::on_pushButtonStart_clicked()
{
    player->setURL(configureDialog->url);


    if(player->Init()) {
        player->Start();
    }else  {
        QString errMsg = "open url:" + QString::fromStdString(configureDialog->url)+" failed";
        QMessageBox::about(NULL, "error",errMsg);
    }

}

void MainWindow::on_pushButtonEnd_clicked()
{
    player->Stop();
}
