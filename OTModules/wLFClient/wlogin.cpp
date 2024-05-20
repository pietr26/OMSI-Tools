#include "wlogin.h"
#include "ui_wlogin.h"

wLogin::wLogin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wLogin)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("Control center trip") + " - " + tr("login"));

    qInfo().noquote() << objectName() + " started";
}

wLogin::~wLogin()
{
    delete ui;
}

void wLogin::on_pushButton_2_clicked()
{
    close();
}


void wLogin::on_actionClose_triggered()
{
    close();
}

