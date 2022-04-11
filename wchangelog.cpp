#include "wchangelog.h"
#include "ui_wchangelog.h"

wChangelog::wChangelog(QWidget *parent, bool updateAvailable) :
    QMainWindow(parent),
    ui(new Ui::wChangelog)
{
    qInfo().noquote() << "Starting " + moduleName + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    setWindowTitle(OTName + " - " + tr("changelog"));

    ui->teedChangelog->setText(tr("Loading..."));

    if (!updateAvailable)
    {
        ui->btnUpdateNow->setVisible(false);
        ui->lblNewUpdate->setVisible(false);
    }

    QTimer::singleShot(0, this, SLOT(downloadChangelog()));

    qInfo().noquote() << moduleName + " started";
}

wChangelog::~wChangelog()
{
    delete ui;
}

/// \brief Downloads current changelog
void wChangelog::downloadChangelog()
{
    QString changelog = dl.doDownload(OTLinks::changelog);
    if ((dl.lastHttpCode != 0) || (dl.lastHttpCode >= 300))
        ui->teedChangelog->setHtml(changelog);
    else
        ui->teedChangelog->setText(tr("There was an error while get the changelog. Please check if your computer has a working internet connection, retry it or contact the developer.\nCode: %1").arg(dl.lastHttpCode));
}

/// \brief Closes the window
void wChangelog::on_btnClose_clicked()
{
    close();
}

/// \brief Calls the settings and execute the update
void wChangelog::on_btnUpdateNow_clicked()
{
    misc.searchForUpdates(this);
}

