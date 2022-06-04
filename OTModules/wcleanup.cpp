#include "wcleanup.h"
#include "ui_wcleanup.h"

wCleanup::wCleanup(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wCleanup)
{
    qInfo().noquote() << "Starting " + moduleName + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load settings
    setStyleSheet(set.read("main","theme").toString());
    setWindowTitle(OTName + " - " + tr("cleanup"));


    qInfo().noquote() << moduleName + " started";
}

wCleanup::~wCleanup()
{
    delete ui;
}
