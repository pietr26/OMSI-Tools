#include "wabout.h"
#include "ui_wabout.h"

wAbout::wAbout(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wAbout)
{
    qInfo().noquote() << "Starting " + moduleName + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setWindowTitle(tr("About") + " " + OTName);

    ui->lblTitle->setText(OTName);
    ui->lblVersion->setText(tr("Version") + " " + OTVersion);

    ui->lbladditionalVersion->setText(appBuild + "<br/><br/>");

    ui->lblInformation->setText(tr("%1 is a tool for creating modifications for OMSI 2 - the bus simulator. %1 makes the creation of modifications easier and it includes helpful features to increase the workflow.").arg(OTName) + "<br/>" + tr("The Project has been started at January 22, 2021 and it's still in an early state of development.") + "<br/><br/>" + tr("%1 is licensed under %2.").arg(OTName, "<a style=\"color: lightblue\" href=\"https://creativecommons.org/licenses/by-nc-nd/4.0/\">Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International (CC BY-NC-ND 4.0)</a>"));

    QString credits = QString("<b>%1:</b> Bamp").arg(tr("Founder & head", "Meaning of 'head': A management role")) +
                      QString("<br/><b>%1:</b> Dari19 | der_Nik_ | DerGrafikfehler | Erilambus | fOcUs04 | MeerrettichMeister | PingPong | SGTVP | TobiB").arg(tr("Beta test")) +
                      QString("<br/><br/><b>%1:</b><br/>").arg(tr("Translators")) +
                      QString("Dari19 (%1)<br/>").arg(tr("italian")) +
                      QString("PG_97 (%1)<br/>").arg(tr("french")) +
                      QString("tfc (%1)<br/>").arg(tr("czech"));

    ui->teedAbout->setHtml(credits);

    qInfo().noquote() << moduleName + " started";
}

wAbout::~wAbout()
{
    delete ui;
}

void wAbout::on_btnClose_clicked()
{
    close();
}


void wAbout::on_btnMore_clicked()
{
    QMessageBox::information(this, tr("More information", "Note #1"), tr("Developed with %1 and %2").arg("Qt 6.2.3", "MinGW 11.2.0") + "\n" +
                                                                         tr("Build type: %1").arg(OTBuildOptions::getBuildName(OTBuild)) + "\n" +
                                                                         tr("Application build: %1").arg(appBuild) + "\n" +
                                                                         tr("Approximate lines of code: %1 (%2)").arg("18000", OTTranslations::getMonthName(4) + "2022"));
}

