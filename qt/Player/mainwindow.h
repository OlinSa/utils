#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPaintEvent>
#include <QWidget>
#include <QtDebug>
#include "videoplayer.h"
#include "configuredialog.h"

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
    ConfigureDialog *configureDialog;

    VIDEO_COLOR_T videoColor;

private slots:
    void slotGetOneFrame(QImage);
    void slotGetRFrame(QImage);
    bool slotClickRed();
    bool slotClickGreen();
    bool slotClickBlue();
    bool slotClickClose();
    bool slotURL();
    void on_pushButtonStart_clicked();
    void on_pushButtonEnd_clicked();
};

#endif // MAINWINDOW_H
