#ifndef WSTARTUPSCREEN_H
#define WSTARTUPSCREEN_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTWidgets/firstsetup.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "OTModules/OTGeneric/wstart.h"
#include "OTWidgets/newupdate.h"
#include "OTWidgets/invalidmaindir.h"

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

    void updateCheck();

private:
    Ui::wStartUpScreen *ui;
    OTSettings set;
    OTMiscellaneous misc;
    OTUpdater *updater = new OTUpdater();
    wStart *WSTART;
    firstSetup *FIRSTSETUP;
    newUpdate *NEWUPDATE;
    invalidMainDir *INVALIDMAINDIR;

    QTimer *timer;

#ifdef QT_DEBUG
    const int startUpScreenDuration = 0;
#else
    const int startUpScreenDuration = 3000;
#endif

    QString updateVersion;
    QPair<int, QString> update;
};

#endif // WSTARTUPSCREEN_H
