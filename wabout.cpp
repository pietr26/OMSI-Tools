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

    setWindowTitle(tr("About") + " " + OTName);

#ifdef QT_DEBUG
    QString build = "Debug";
#else
    QString build = "Release";
#endif

    QString aboutText =         QString("<h2>%1</h2>").arg(OTName) +
                                tr("%1 is a tool for creating modifications for OMSI 2 - the bus simulator. %1 makes the creation of modifications easier and it includes helpful features to increase the workflow.").arg(OTName) + "<br>" + tr("The Project has been started at January 22, 2021.") + "<br><br>" +
            "<table><tr><td><b>" +    tr("Version:") +           "</b></td><td>" + OTVersion +
            "</td></tr><tr><td><b>" + tr("Official Build:") +    "</b></td><td>" + OTBuildOptions::getBuildName(OTBuild) +
            "</td></tr><tr><td><b>" + tr("Application Build:") + "     </b></td><td>" + build + "</td></tr></table><br><br>" +
            "<b>" +                   tr("Licensed under:") +    "</b> CC-BY-NC-ND</a> (https://creativecommons.org/licenses/by-nc-nd/4.0/)" + "<br><br><b>" +
            tr("Involved persons:") + "</b>" +

            // Involved persons:
                          "<ul>" +
                                QString("<li><b>Bamp</b> (%1)</li>").arg(tr("founder & head", "Meaning of 'head': A management role")) +
                                QString("<li><b>Dari19</b> | <b>der_Nik_</b> | <b>DerGrafikfehler</b> | <b>Erilambus</b> | <b>fOcUs04</b> | <b>MeerrettichMeister</b> | <b>PingPong</b> | <b>SGTVP</b> | <b>TobiB</b> (%1)</li>").arg(tr("beta test")) +
                                QString("<li><b>Dari19</b> (%1)</li>").arg(tr("italian translations")) +
                                QString("<li><b>PG_97</b> (%1)</li>").arg(tr("french translations")) + "</ul>";

    ui->teedAbout->setHtml(aboutText);

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    qInfo().noquote() << moduleName + " started successfully.";
}

wAbout::~wAbout()
{
    delete ui;
}
