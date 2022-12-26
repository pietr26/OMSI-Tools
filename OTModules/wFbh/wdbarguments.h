#ifndef WDBARGUMENTS_H
#define WDBARGUMENTS_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTDatabaseHandler.h"
#include <QtSql>

namespace Ui {
class wDBArguments;
}

class wDBArguments : public QMainWindow
{
    Q_OBJECT

public:
    explicit wDBArguments(QWidget *parent = nullptr);
    ~wDBArguments();

private:
    Ui::wDBArguments *ui;

    OTSettings set;
    OTMiscellaneous misc;

    QString dbPath = "C:/Users/pietr/OneDrive/Dev/OMSI-Tools/OMSI-Tools/data/db/fbhCopyrights.db";
    OTDatabaseHandler dbHandler;
};

#endif // WDBARGUMENTS_H
