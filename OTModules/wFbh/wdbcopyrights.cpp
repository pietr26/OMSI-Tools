#include "wdbcopyrights.h"
#include "ui_wdbcopyrights.h"

wDBCopyrights::wDBCopyrights(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wDBCopyrights)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - argument panel");

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    // Setup database
    dbHandler.dbPath = dbPath;
    if (!QFile(QDir(dbPath).absolutePath()).exists())
    {
        dbHandler.setupDatabase(true);
        dbHandler.doAction("CREATE TABLE arguments (ID INTEGER, argument TEXT, PRIMARY KEY(ID AUTOINCREMENT))");
        dbHandler.doAction("CREATE TABLE paths (ID INTEGER, path TEXT, argumentIDs TEXT, PRIMARY KEY(ID AUTOINCREMENT))");
    }
    else
        dbHandler.setupDatabase();

    qInfo().noquote() << objectName() + " started";
}

wDBCopyrights::~wDBCopyrights()
{
    delete ui;
}
