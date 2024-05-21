#ifndef WLOGIN_H
#define WLOGIN_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wLogin;
}

class wLogin : public QMainWindow
{
    Q_OBJECT

public:
    explicit wLogin(QWidget *parent = nullptr);
    ~wLogin();

    QString username() const;
    QString password() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

signals:
    void accepted();

private:
    Ui::wLogin *ui;
};

#endif // WLOGIN_H
