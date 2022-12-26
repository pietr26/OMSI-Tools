#ifndef WDBCOPYRIGHTS_H
#define WDBCOPYRIGHTS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include <QtSql>

namespace Ui {
class wDBCopyrights;
}

class wDBCopyrights : public QMainWindow
{
    Q_OBJECT

public:
    explicit wDBCopyrights(QWidget *parent = nullptr);
    ~wDBCopyrights();

private:
    Ui::wDBCopyrights *ui;

    OTSettings set;
    OTMiscellaneous misc;

    QString dbPath = "C:/Users/pietr/OneDrive/Dev/OMSI-Tools/OMSI-Tools/data/db/fbhCopyrights.db";
    OTDatabaseHandler dbHandler;
};

#endif // WDBCOPYRIGHTS_H
