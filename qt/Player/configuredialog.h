#ifndef CONFIGUREDIALOG_H
#define CONFIGUREDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class ConfigureDialog;
}

class ConfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureDialog(QWidget *parent = nullptr);
    ~ConfigureDialog();

private slots:
    void on_buttonBox_accepted();

public:
    std::string url;
private:
    Ui::ConfigureDialog *ui;

};

#endif // CONFIGUREDIALOG_H
