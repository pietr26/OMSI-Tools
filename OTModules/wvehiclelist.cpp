#include "wvehiclelist.h"
#include "ui_wvehiclelist.h"

/////////////////////////////////////////////////
/// ATTENTION ///////////////////////////////////
/// This module is outdated. ////////////////////
/////////////////////////////////////////////////

wVehicleList::wVehicleList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wVehicleList)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("vehicle lists"));

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    qInfo().noquote() << objectName() + " started";
}

wVehicleList::~wVehicleList()
{
    delete ui;
}

/// Opens the prefs
void wVehicleList::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences(this);
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

/// Closes the window
void wVehicleList::on_actionClose_triggered()
{
    QApplication::quit();
}

/// database test function
void wVehicleList::dbTest()
{
    if (!QDir("databases").exists())
        QDir().mkdir("databases");

    QString path = "databases/test.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    for (int i = 0; i < 20; i++)
        query.exec("create table testTable "
                   "(ID integer primary key, "
                   "testString varchar(20), "
                   "testInt integer)");
}

/// Opens the database
void wVehicleList::openDatabase(QString filename)
{
    Q_UNUSED(filename);
    // code destroyed
//    databaseHandler database(filename);
//    database.database = QSqlDatabase::addDatabase("QSQLITE");

}

/// Rund database test function (redirect)
void wVehicleList::on_btnRunDBFunction_clicked()
{
    dbTest();
}

/// Opens a database
void wVehicleList::on_actionOpen_triggered()
{
    openDatabase(QFileDialog::getOpenFileName(this, tr("Select database"), "vehListDatabases", tr("Databases") + " (*.db)"));
}

