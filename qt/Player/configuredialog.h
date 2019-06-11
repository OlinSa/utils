#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class ConfigureDialog;
}

typedef enum {
    MEDIA_RTSP = 0,
    MEDIA_RTMP
}MEDIA_PROTO_T;

class ConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureDialog(QWidget *parent = nullptr);
    ~ConfigureDialog();

private slots:
    void on_buttonBox_accepted();

    void on_pushButtonSetPath_clicked();

public:
    std::string url;
    MEDIA_PROTO_T mediaProto;
private:
    Ui::ConfigureDialog *ui;

};

#endif // CONFIGUREDIALOG_H
