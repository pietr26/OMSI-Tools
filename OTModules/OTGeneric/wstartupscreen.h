#ifndef WSTARTUPSCREEN_H
#define WSTARTUPSCREEN_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTWidgets/firstsetup.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "OTModules/OTGeneric/wstart.h"

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
    void finished();

    void openWStart();

    void on_btnClose_clicked();

private:
    Ui::wStartUpScreen *ui;
    OTSettings set;
    wStart *WSTART;
    firstSetup *FIRSTSETUP;

    QTimer *timer;

#ifdef QT_DEBUG
    const int startUpScreenDuration = 0;
#else
    const int startUpScreenDuration = 3000;
#endif
};

#endif // WSTARTUPSCREEN_H
