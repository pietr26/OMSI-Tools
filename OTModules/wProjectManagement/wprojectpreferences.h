#ifndef WPROJECTPREFERENCES_H
#define WPROJECTPREFERENCES_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTExternal.h"

namespace Ui {
class wProjectPreferences;
}

class wProjectPreferences : public QMainWindow
{
    Q_OBJECT

public:
    explicit wProjectPreferences(QString projectFolder, QWidget *parent = nullptr);
    ~wProjectPreferences();

private slots:
    void on_btnSave_clicked();

    void on_btnClose_clicked();

private:
    Ui::wProjectPreferences *ui;
    OTSettings set;
    OTGit git;
};

#endif // WPROJECTPREFERENCES_H
