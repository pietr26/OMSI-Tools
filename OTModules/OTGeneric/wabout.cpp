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

    // Load prefs
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setWindowTitle(tr("About") + " " + OTInformation::name);

    ui->lblTitle->setText(OTInformation::name);
    ui->lblVersion->setText(tr("Version") + " " + OTInformation::versions::currentVersion.first);

    ui->lblAdditionalVersion->setText(QString("%1 (%2)<br/><br/>").arg(appBuild).arg(OTInformation::versions::currentVersion.second));

    QString generalInformation = tr("%1 is a tool for creating modifications for OMSI 2 - the bus simulator. %1 makes the creation of modifications easier and it includes helpful features to increase the workflow.").arg(OTInformation::name) + "<br/>" + tr("The Project has been started at January 22, 2021 and it's still in an early state of development.") + "<br/><br/>" + tr("%1 is licensed under the %2.").arg(OTInformation::name, "<a style=\"color: lightblue\" href=\"https://www.gnu.de/documents/gpl-2.0.en.html\">GNU General Public License v2.0</a>");

    QString credits = QString("<b>%1:</b>").arg(tr("Development")) +
                      QString("<ul><li>Bamp <i>(%1)</i></li>").arg(tr("Management, development")) +
                      QString("<li>ma7t3 <i>(%1)</i></li></ul>").arg(tr("development")) +
                      QString("<b>%1:</b>").arg(tr("Beta testers")) +
                      QString("<ul><li>Dari19</li>"
                              "<li>der_Nik_</li>"
                              "<li>DerGrafikfehler</li>"
                              "<li>Erilambus</li>"
                              "<li>fOcUs04</li>"
                              "<li>Lenn</li>"
                              "<li>MeerrettichMeister</li>"
                              "<li>PingPong</li>"
                              "<li>ma7t3</li>"
                              "<li>TobiB</li></ul>") +
                      QString("<b>%1:</b>").arg(tr("Translators")) +
                      QString("<ul><li>Dari19 <i>(%1)</i></li>").arg(tr("italian")) +
                      QString("<li>Erilambus <i>(%1)</i></li>").arg(tr("french")) +
                      QString("<li>tfc <i>(%1)</i></li></ul>").arg(tr("czech"));

    ui->lblInformation->setText(generalInformation + "<br/><br/><br/>");
    ui->lblCredits->setText(credits);

    qInfo().noquote() << objectName() + " started";
}

wAbout::~wAbout()
{
    delete ui;
}

/// Closes the window
void wAbout::on_btnClose_clicked()
{
    close();
}

/// Shows detailed information (framework etc.)
void wAbout::on_btnMore_clicked()
{
    QMessageBox::about(this, tr("More information"), tr("Build type: %1").arg(OTBuildOptions::getBuildName(OTInformation::build)) + "\n" +
                                                     tr("Application build: %1").arg(appBuild) + "\n\n" +
                                                     tr("Approximate lines of code: %1 (%2)").arg(OTInformation::sourceCodeLength, tr("May 2024")));
}

void wAbout::on_btnFlowerella_clicked()
{
    if ((devToolsCount == 0) && set.read("main", "devToolsEnabled").toBool()) ui->statusbar->showMessage("DevTools menu is already enabled! :-)");

    devToolsCount++;
    if ((devToolsCount >= 15) && !set.read("main", "devToolsEnabled").toBool()) {
        set.write("main", "devToolsEnabled", true);
        ui->statusbar->showMessage("DevTools menu is enabled now. Restart to apply.");
    }
}
