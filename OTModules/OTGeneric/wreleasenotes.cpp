#include "wreleasenotes.h"
#include "ui_wreleasenotes.h"

wReleaseNotes::wReleaseNotes(QWidget *parent, bool updateAvailable, QString newVersion, bool viaUpdater) :
    QMainWindow(parent),
    ui(new Ui::wReleaseNotes)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("release notes"));

    ui->statusbar->addPermanentWidget(ui->lblSelectBranch);
    ui->statusbar->addPermanentWidget(ui->cbxBranch);

    qApp->processEvents();
    ui->teedReleaseNotes->setText(tr("Loading..."));

    QStringList branches = QString(nc.post(QUrl(OTLinks::versionBranches))).split("\n");
    branches.removeAll(QString(""));
    ui->cbxBranch->addItems(branches);

    ui->btnUpdateNow->setEnabled(viaUpdater);
    ui->lblDownloadManually->setVisible(!viaUpdater);

    if (!updateAvailable)
    {
        ui->lblDownloadManually->setVisible(false);
        ui->btnUpdateNow->setVisible(false);
        ui->btnUpdateManually->setVisible(false);
        ui->lblNewUpdate->setVisible(false);
        ui->lblNewVersion->setVisible(false);
    }

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->pgbProgress->setVisible(false);

    ui->lblDownloadManually->setText(tr("Since you installed a portable version, you have to update %1 manually.").arg(OTInformation::name));

    ui->lblCurrentVersion->setText("<b>" + tr("Current version:") + "</b> " + OTInformation::versions::currentVersion.first);

    if (newVersion != "")
        ui->lblNewVersion->setText("<b>" + tr("New version:") + "</b> " + newVersion);
    else
        ui->lblNewVersion->setVisible(false);

    setupFinished = true;
    QTimer::singleShot(0, this, SLOT(downloadReleaseNotes()));

    qInfo().noquote() << objectName() + " started";
}

wReleaseNotes::~wReleaseNotes()
{
    delete ui;
}

/// Downloads current release notes
void wReleaseNotes::downloadReleaseNotes(QString version)
{
    ui->teedReleaseNotes->setText(tr("Loading..."));

    if (version == "") version = ui->cbxBranch->currentText();

    QString releaseNotes = nc.post(QUrl(OTLinks::releaseNotes + "&lang=" + set.getCurrentLanguageCode() + "&version=" + version));
    if (nc.lastSuccess == 1)
        ui->teedReleaseNotes->setHtml(releaseNotes);
    else if (nc.lastSuccess == -2)
        ui->teedReleaseNotes->setText(OTStrings::serverMaintenance());
    else
        ui->teedReleaseNotes->setText(tr("The release notes couldn't be downloaded. Please check if your computer has a working internet connection, retry it or contact the developer.\nHTTP %1").arg(nc.lastHttpCode));
}

/// Closes the window
void wReleaseNotes::on_btnClose_clicked()
{
    close();
}

/// Calls the prefs and execute the update
void wReleaseNotes::on_btnUpdateNow_clicked()
{
    QMessageBox::information(this, tr("Install update"), tr("%1 closes for installing the new version. Please wait until %1 has been restarted.\nDepending on the computer performance and the internet connection, this process may take a moment.").arg(OTInformation::name));

    ui->centralwidget->setEnabled(false);
    ui->pgbProgress->setVisible(true);
    ui->statusbar->showMessage(tr("Start update..."));

    updater->start();
}

void wReleaseNotes::on_cbxBranch_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (setupFinished)
        downloadReleaseNotes(ui->cbxBranch->currentText());
}

void wReleaseNotes::on_btnUpdateManually_clicked()
{
    QDesktopServices::openUrl(OTLinks::GitHub::releases);
}

void wReleaseNotes::on_actionClose_triggered()
{
    close();
}

