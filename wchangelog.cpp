#include "wchangelog.h"
#include "ui_wchangelog.h"

wChangelog::wChangelog(QWidget *parent, bool updateAvailable, QString newVersion) :
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
        ui->lblNewVersion->setVisible(false);
        ui->lblCurrentVersion->setVisible(false);
        ui->cbxClearAppDir->setVisible(false);
    }

    ui->lblClearAppDirInfo->setVisible(false);

    ui->lblCurrentVersion->setText("<b>" + tr("Current version:") + "</b> " + OTVersion);

    if (newVersion != "")
        ui->lblNewVersion->setText("<b>" + tr("New version:") + "</b> " + newVersion);
    else
        ui->lblNewVersion->setVisible(false);

    QTimer::singleShot(0, this, SLOT(downloadChangelog()));

    qInfo().noquote() << moduleName + " started";
}

wChangelog::~wChangelog()
{
    delete ui;
}

/// Downloads current changelog
void wChangelog::downloadChangelog()
{
    QString changelog = dl.doDownload(OTLinks::changelog);
    if ((dl.lastHttpCode != 0) || (dl.lastHttpCode >= 300))
        ui->teedChangelog->setHtml(changelog);
    else
        ui->teedChangelog->setText(tr("There was an error while get the changelog. Please check if your computer has a working internet connection, retry it or contact the developer.\nCode: %1").arg(dl.lastHttpCode));
}

/// Closes the window
void wChangelog::on_btnClose_clicked()
{
    close();
}

/// Calls the settings and execute the update
void wChangelog::on_btnUpdateNow_clicked()
{
    misc.startUpdate(this, ui->cbxClearAppDir->isChecked());
}

void wChangelog::on_cbxClearAppDir_stateChanged(int arg1)
{
    if (arg1 == 2)
        ui->lblClearAppDirInfo->setVisible(true);
    else
        ui->lblClearAppDirInfo->setVisible(false);
}

