#include "wfeedback.h"
#include "ui_wfeedback.h"

wFeedback::wFeedback(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wFeedback)
{
    qInfo().noquote() << "Starting " + moduleName + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    // Load settings
    setStyleSheet(set.read("main","theme").toString());
    setWindowTitle(OTName + " - " + tr("feedback"));

    ui->btnOpenSupportThread->setIconSize(QSize(50, 50));

    qInfo().noquote() << moduleName + " started";
}

wFeedback::~wFeedback()
{
    delete ui;
}

void wFeedback::on_btnOpenSupportThread_clicked()
{
    QDesktopServices::openUrl(OTLinks::support);
    close();
}

void wFeedback::on_btnOpenSurvey_clicked()
{
    QDesktopServices::openUrl(OTLinks::survey);
    close();
}

