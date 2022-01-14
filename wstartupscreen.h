#ifndef WSTARTUPSCREEN_H
#define WSTARTUPSCREEN_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include <QPropertyAnimation>
#include "wstart.h"

namespace Ui {
class wStartUpScreen;
}

class wStartUpScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit wStartUpScreen(QWidget *parent = nullptr);
    ~wStartUpScreen();

private slots:
    void goToWStart();

private:
    const QString moduleName = "wStartUpScreen";
    Ui::wStartUpScreen *ui;
    OTSettings set;
    wStart *WSTART;

    QTimer *timer;

#ifdef QT_DEBUG
    const int startUpScreenDuration = 0;
#else
    const int startUpScreenDuration = 3000;
#endif
};

#endif // WSTARTUPSCREEN_H
