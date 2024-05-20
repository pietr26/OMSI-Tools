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

private slots:
    void on_pushButton_2_clicked();

    void on_actionClose_triggered();

private:
    Ui::wLogin *ui;
};

#endif // WLOGIN_H
