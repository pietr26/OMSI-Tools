#include "wabout.h"
#include "ui_wabout.h"

wAbout::wAbout(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wAbout)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    resize(misc.sizeWindow(0.5, 0.9));
    move(misc.centerPosition(this));
    qDebug() << "UI set";

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setWindowTitle(tr("About") + " " + OTName);

    ui->lblTitle->setText(OTName);
    ui->lblVersion->setText(tr("Version") + " " + OTVersion);

    ui->lblAdditionalVersion->setText(appBuild + "<br/><br/>");

    QString generalInformation = tr("%1 is a tool for creating modifications for OMSI 2 - the bus simulator. %1 makes the creation of modifications easier and it includes helpful features to increase the workflow.").arg(OTName) + "<br/>" + tr("The Project has been started at January 22, 2021 and it's still in an early state of development.") + "<br/><br/>" + tr("%1 is licensed under the %2.").arg(OTName, "<a style=\"color: lightblue\" href=\"https://www.gnu.de/documents/gpl-2.0.en.html\">GNU General Public License v2.0</a>");

    QString credits = QString("<b>%1:</b> Bamp").arg(tr("Development")) +
                      QString("<br/><b>%1:</b> ma7t3").arg(tr("Web design")) +
                      QString("<br/><br/><b>%1:</b> Dari19 | der_Nik_ | DerGrafikfehler | Erilambus | fOcUs04 | MeerrettichMeister | PingPong | ma7t3 | TobiB").arg(tr("Beta test")) +
                      QString("<br/><br/><b>%1:</b><br/>").arg(tr("Translators")) +
                      QString("Dari19 (%1)<br/>").arg(tr("italian")) +
                      QString("PG_97 (%1)<br/>").arg(tr("french")) +
                      QString("tfc (%1)").arg(tr("czech"));

    ui->lblInformation->setText(generalInformation + "<br/><br/><br/>");
    ui->lblCredits->setText(credits);

    qInfo().noquote() << objectName() + " started";
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
    QMessageBox::about(this, tr("More information"), tr("Developed with %1 and %2").arg("Qt 6.3.0", "MinGW 11.2.0") + "\n\n" +
                                                                         tr("Build type: %1").arg(OTBuildOptions::getBuildName(OTBuild)) + "\n" +
                                                                         tr("Application build: %1").arg(appBuild) + "\n\n" +
                                                                         tr("Approximate lines of code: %1 (%2)").arg("18 000", OTStrings::getMonthName(4) + " 2022"));
}

