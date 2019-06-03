#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPaintEvent>
#include <QWidget>
#include <QtDebug>
#include "videoplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;

    VideoPlayer *player;
    QImage image;
    QImage rImage;
    std::string url;
    bool openRed;

private slots:
    void slotGetOneFrame(QImage);
    void slotGetRFrame(QImage);
    bool slotOpenRed();
    bool slotCloseRed();
};

#endif // MAINWINDOW_H
