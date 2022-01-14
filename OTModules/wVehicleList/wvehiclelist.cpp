#include "wvehiclelist.h"
#include "ui_wvehiclelist.h"

wVehicleList::wVehicleList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wVehicleList)
{
    qInfo().noquote() << "Starting " + moduleName + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("vehicle lists"));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    qInfo().noquote() << moduleName + " started successfully.";
}

wVehicleList::~wVehicleList()
{
    qInfo().noquote() << moduleName + " is closing...";
    delete ui;
}

/// \brief Opens the settings
void wVehicleList::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings(this);
    WSETTINGS->setWindowModality(Qt::ApplicationModal);
    WSETTINGS->show();
}

/// \brief Closes the window
void wVehicleList::on_actionClose_triggered()
{
    QApplication::quit();
}

/// \brief database test function
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

/// \brief Opens the database
void wVehicleList::openDatabase(QString filename)
{
    Q_UNUSED(filename);
    // code destroyed
//    databaseHandler database(filename);
//    database.database = QSqlDatabase::addDatabase("QSQLITE");

}

/// \brief Rund database test function (redirect)
void wVehicleList::on_btnRunDBFunction_clicked()
{
    dbTest();
}

/// \brief Opens a database
void wVehicleList::on_actionOpen_triggered()
{
    openDatabase(QFileDialog::getOpenFileName(this, tr("Select database"), "vehListDatabases", tr("Databases") + " (*.db)"));
}

/// \brief Opens bug report module
void wVehicleList::on_actionSendFeedback_triggered()
{
    OTMiscellaneous::sendFeedback();
}

