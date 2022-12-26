#include "wdbarguments.h"
#include "ui_wdbarguments.h"

wDBArguments::wDBArguments(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wDBArguments)
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
//        dbHandler.setupDatabase(true);
//        dbHandler.doAction("CREATE TABLE paths (ID INTEGER, path varchar(5000), linkID varchar(5000), PRIMARY KEY(ID AUTOINCREMENT))");
//        dbHandler.doAction("CREATE TABLE links (ID INTEGER, link varchar(5000), directLinks varchar(5000), information varchar(5000),PRIMARY KEY(ID AUTOINCREMENT))");
    }
    else
        dbHandler.setupDatabase();

    qInfo().noquote() << objectName() + " started";
}

wDBArguments::~wDBArguments()
{
    delete ui;
}
