#include "configuredialog.h"
#include "ui_configuredialog.h"

ConfigureDialog::ConfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureDialog)
{
    ui->setupUi(this);
    url = "rtsp://192.168.124.128:8554/test.264";
    ui->uri->setText(QString::fromStdString(url));
}

ConfigureDialog::~ConfigureDialog()
{
    delete ui;
}

void ConfigureDialog::on_buttonBox_accepted()
{
//    if(url != ui->uri->text().toStdString()) {
        url = ui->uri->text().toStdString();

//    }
}
