#ifndef WVERIFYCATIONSETTINGS_H
#define WVERIFYCATIONSETTINGS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wVerifycationSettings;
}

class wVerifycationSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit wVerifycationSettings(QWidget *parent = nullptr);
    ~wVerifycationSettings();

private slots:
    void on_btnClose_clicked();

    void on_cbxAdvancedVerifying_stateChanged(int arg1);

    void on_cbxOnlyMapTextures_stateChanged(int arg1);

private:
    Ui::wVerifycationSettings *ui;
    const QString moduleName = "wVerifyMap";
    OTSettings set;
};

#endif // WVERIFYCATIONSETTINGS_H
