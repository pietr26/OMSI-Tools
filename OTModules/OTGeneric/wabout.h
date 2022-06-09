#ifndef WABOUT_H
#define WABOUT_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wAbout;
}

class wAbout : public QMainWindow
{
    Q_OBJECT

public:
    explicit wAbout(QWidget *parent = nullptr);
    ~wAbout();

private slots:
    void on_btnClose_clicked();

    void on_btnMore_clicked();

private:
    Ui::wAbout *ui;
    OTSettings set;
    OTMiscellaneous misc;

#ifdef QT_DEBUG
    QString appBuild = "Debug";
#else
    QString appBuild = "Release";
#endif
};

#endif // WABOUT_H
