#ifndef WVEHICLELIST_H
#define WVEHICLELIST_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include <QtSql>
#include <QInputDialog>

/////////////////////////////////////////////////
/// ATTENTION ///////////////////////////////////
/// This module is outdated. ////////////////////
/////////////////////////////////////////////////

namespace Ui {
class wVehicleList;
}

class wVehicleList : public QMainWindow
{
    Q_OBJECT

public:
    explicit wVehicleList(QWidget *parent = nullptr);
    ~wVehicleList();

private slots:
    void on_actionPreferences_triggered();

    void on_actionClose_triggered();

    void on_btnRunDBFunction_clicked();

    void on_actionOpen_triggered();

private:
    Ui::wVehicleList *ui;

    wPreferences *WPREFERENCES;

    OTSettings set;

    void dbTest();

    void openDatabase(QString filename);
};

#endif // WVEHICLELIST_H
