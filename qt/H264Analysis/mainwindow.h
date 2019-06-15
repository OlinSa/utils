#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "h264_wrap.h"
#include "log.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, int bufSize = 1024);
    ~MainWindow();

    int showMediaInfo(const std::string &filename);
    int insertItem(const NaluUnit &nalu,const NaluParam &naluParam,unsigned long long pos,int id, int row, QBrush brush);
private slots:
    void on_pushButtonStart_clicked();

private:
    Ui::MainWindow *ui;
    H264Wrap *h264Wrap;
    unsigned char *buf;
    int bufSize;
    QStandardItemModel* model;//标准项模型
};

#endif // MAINWINDOW_H
