#include "wreleasenotes.h"
#include "ui_wreleasenotes.h"

wReleaseNotes::wReleaseNotes(QWidget *parent, bool updateAvailable, QString newVersion) :
    QMainWindow(parent),
    ui(new Ui::wReleaseNotes)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    setWindowTitle(OTName + " - " + tr("release notes"));

    ui->teedReleaseNotes->setText(tr("Loading..."));

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

    QTimer::singleShot(0, this, SLOT(downloadReleaseNotes()));

    qInfo().noquote() << objectName() + " started";
}

wReleaseNotes::~wReleaseNotes()
{
    delete ui;
}

/// Downloads current release notes
void wReleaseNotes::downloadReleaseNotes()
{
    QString releaseNotes = dl.doDownload(OTLinks::releaseNotes);
    if ((dl.lastHttpCode != 0) || (dl.lastHttpCode >= 300))
        ui->teedReleaseNotes->setHtml(releaseNotes);
    else
        ui->teedReleaseNotes->setText(tr("The release notes couldn't be downloaded. Please check if your computer has a working internet connection, retry it or contact the developer.\nHTTP %1").arg(dl.lastHttpCode));
}

/// Closes the window
void wReleaseNotes::on_btnClose_clicked()
{
    close();
}

/// Calls the settings and execute the update
void wReleaseNotes::on_btnUpdateNow_clicked()
{
    misc.startUpdate(this, ui->cbxClearAppDir->isChecked());
}

void wReleaseNotes::on_cbxClearAppDir_stateChanged(int arg1)
{
    if (arg1 == 2)
        ui->lblClearAppDirInfo->setVisible(true);
    else
        ui->lblClearAppDirInfo->setVisible(false);
}

