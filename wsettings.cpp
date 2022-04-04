#include "wsettings.h"
#include "ui_wsettings.h"

wSettings::wSettings(QWidget *parent, QString openDirect) :
    QMainWindow(),
    ui(new Ui::wSettings)
{
    qInfo().noquote() << "Starting " + moduleName + "...";
    Q_UNUSED(parent);

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("settings"));
    //ui->statusbar->showMessage(tr("Restart to apply all settings."));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshDiskUsage()));
    timer->start(5000);

    // cobxLanguage
    int languageIndex = set.read("main", "language").toInt();
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/en.svg"), set.langEn); // 0
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/de.svg"), set.langDe); // 1
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fr.svg"), set.langFr); // 2
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/it.svg"), set.langIt); // 3
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cz.svg"), set.langCz); // 4

    // Disable 'it'
    auto* model3 = qobject_cast<QStandardItemModel*>(ui->cobxLanguage->model());
    model3->item(3)->setEnabled(false);

    ui->cobxLanguage->setCurrentIndex(languageIndex);

    // cobxTheme:
    int themeIndex = set.read("main", "theme", true).toInt();
    ui->cobxTheme->addItem(QString("Standard"));
    ui->cobxTheme->addItem(QString("Combinear"));
    ui->cobxTheme->addItem(QString("Darkeum"));

    ui->cobxTheme->setCurrentIndex(themeIndex);

    // cbxAutoSave
    if (set.read("main", "autosave") == "true")
        ui->gbxAutosave->setChecked(true);
    else if (set.read("main", "autosave") == "false")
        ui->gbxAutosave->setChecked(false);

    // lblDiskUsage
    ui->lblDiskUsage->setText(tr("Backup disk usage") + tr("Calculating..."));
    refreshDiskUsage();

    // sbxAutosaveDuration
    QString autosaveDuration = set.read("main", "autosaveDuration").toString();
    if (!autosaveDuration.isEmpty())
        ui->sbxAutosaveDuration->setValue(set.read("main", "autosaveDuration").toInt());

    ui->sbxAutosaveDuration->setSuffix(" " + tr("sec.", "short form from 'seconds'"));

    // ledAuthor
    ui->ledAuthor->setText(set.read("main", "author").toString());

    // ledOmsiPath
    ui->ledOmsiPath->setText(set.read("main", "mainDir").toString());

    //cbxConfirmDeletion
    if (set.read("main", "confirmDeletion") == "true")
        ui->cbxConfirmDeletion->setChecked(true);
    else if (set.read("main", "confirmDeletion") == "false")
        ui->cbxConfirmDeletion->setChecked(false);

    // cobxLogfileMode
    QStringList logfileModes;
    //                -1            0              1          2
    logfileModes << tr("Off") << tr("Standard") << "Debug" << "Debug+";
    ui->cobxLogfileMode->addItems(logfileModes);
    switch (set.read("main", "logfileMode").toInt())
    {
        case -1: ui->cobxLogfileMode->setCurrentIndex(0); break;
        case 0: ui->cobxLogfileMode->setCurrentIndex(1); break;
        case 1: ui->cobxLogfileMode->setCurrentIndex(2); break;
        case 2: ui->cobxLogfileMode->setCurrentIndex(3); break;
    }

    if (!set.read("main", "logfileMode").isValid())
        ui->cobxLogfileMode->setCurrentIndex(1);

    // cobxAutoUpdateCheck
    QStringList updateModes;
    //                0            1                 2              3               4
    updateModes << tr("Off") << tr("On start") << tr("Daily") << tr("Weekly") << tr("Monthly");
    ui->cobxAutoUpdateCheck->addItems(updateModes);
    switch (set.read("main", "autoUpdateCheck").toInt())
    {
        case 0: ui->cobxAutoUpdateCheck->setCurrentIndex(0); break;
        case 1: ui->cobxAutoUpdateCheck->setCurrentIndex(1); break;
        case 2: ui->cobxAutoUpdateCheck->setCurrentIndex(2); break;
        case 3: ui->cobxAutoUpdateCheck->setCurrentIndex(3); break;
        case 4: ui->cobxAutoUpdateCheck->setCurrentIndex(4); break;
    }

    if (!set.read("main", "autoUpdateCheck").isValid())
        ui->cobxAutoUpdateCheck->setCurrentIndex(2);

    setupFinished = true;

    setUnsaved(false);

    // Open direct modes
    if (openDirect == "mainDirSelection")
    {
        on_btnOmsiPath_clicked();
        QTimer::singleShot(0, this, SLOT(close()));
    }
    else if (openDirect == "checkForUpdate")
        QTimer::singleShot(0, this, SLOT(on_btnCheckForUpdates_clicked()));


    qInfo().noquote() << moduleName + " started successfully.";
}

wSettings::~wSettings()
{
    delete ui;
}

void wSettings::closeEvent (QCloseEvent *event)
{
    Q_UNUSED(event);

    qInfo().noquote() << moduleName + " is closing...";
}

/// \brief Sets unsaved
void wSettings::setUnsaved(bool visible)
{
    if (visible)
        ui->statusbar->showMessage(tr("Restart to apply all settings."));
    else
        ui->statusbar->showMessage("", 10);

    ui->btnRestart->setVisible(visible);
}

/// \brief Refreshes disk usage for backup folder
void wSettings::refreshDiskUsage()
{
    ui->lblDiskUsage->setText(tr("Backup disk usage") + ": " + dUs.formatSize("backup"));
}

/// \brief Closes the application
void wSettings::on_btnClose_clicked()
{
    close();
}

/// \brief Deletes the backup folder
void wSettings::on_btnDeleteAllBackups_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Delete all backups", "Note #1"), tr("Should all backups be deleted? They will be moved to the recycle bin."));

    if (reply == QMessageBox::Yes)
    {
        qDebug() << "Move to trash:" << QFile::moveToTrash("backup");
        ui->btnDeleteAllBackups->setEnabled(false);
        refreshDiskUsage();
        qInfo().noquote() << "Deleted backups";
    }
}

/// \brief Opens the backup folder
void wSettings::on_btnOpenBackupFolder_clicked()
{
    QDir().mkdir("backup");
    QString path = QFileInfo("./backup").absolutePath();
    path += "/backup";

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

/// \brief Saves and restarts the application
void wSettings::on_btnRestart_clicked()
{
//    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Restart now?", "Note #1"), tr("Should the application be restarted now? Any unsaved content will be discarded."));

//    if (reply == QMessageBox::Yes)
        misc.restart();
}

/// \brief Checks for updates
void wSettings::on_btnCheckForUpdates_clicked()
{
    QStringList update = misc.checkForUpdate();
    qDebug().noquote() << "Update from server:" << update;

    if (update.at(0) == "false")
    {
        QMessageBox::warning(this, tr("Error while check version", "Note #1"), tr("There was an error while get the newest version. Please check if your computer has a working internet connection, retry it or contact the developer."));
        qWarning() << "Could not check newest update!";
    }
    else if (update.at(0) == "noUpdates")
    {
        QMessageBox::information(this, tr("Updating %1", "Note #1").arg(OTName), tr("There aren't any updates available."));
        qInfo() << "There's no update available.";
    }
    else
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Updates available", "Note #1"), QString("<html>%1<br><br><b>%2:</b> %3<br><b>%4:</b> %5<br><br>%6</html>").arg(tr("There is an update available."), tr("Installed version"), OTVersion, tr("Newest version"), update.at(1), tr("Should the newest version be installed?")));
        qInfo() << "Updates available!";

        if (reply == QMessageBox::Yes)
        {

            QDirIterator dirIterator(QApplication::applicationDirPath(), QStringList{"*.*"}, QDir::Files, QDirIterator::Subdirectories);
            while (dirIterator.hasNext())
            {
                QString file = dirIterator.next();

                QDir().mkdir(QDir::tempPath() + "/OMSI-Tools_tempAppDir");

                QString newFilePath = QDir::tempPath() + "/OMSI-Tools_tempAppDir/" + file.remove(0, QApplication::applicationDirPath().count() + 1);
                QDir().mkpath(QFileInfo(newFilePath).absolutePath());

                qDebug().noquote() << "New path:" << newFilePath;
                if (QFile(newFilePath).exists())
                    QFile(newFilePath).remove();

                qDebug().noquote() << "From path:" << QApplication::applicationDirPath() + "/" + file;
                qDebug() << "-----------------------------------------------------------------";
                QFile(QApplication::applicationDirPath() + "/" + file).copy(newFilePath);
            }

            QStringList args;
            args << "callFromMainApplication";
            args << QApplication::applicationDirPath();
            if (!QProcess::startDetached(QDir::tempPath() + "/OMSI-Tools_tempAppDir/OMSI-Tools_Updater.exe", args))
            {
                qWarning() << "There was an error while starting the updater (with name 'OMSI-Tools_Updater.exe').";
                QMessageBox::warning(this, tr("Updating %1", "Note #1").arg(OTName), tr("There was an error while starting the updater. Please retry it or conact the developer."));
            }
            else
            {
                qInfo() << QString("Updating %1...").arg(OTName);
                QApplication::quit();

            }
        }
    }
}

/// \brief Resets the settings
void wSettings::on_btnResetSettings_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Reset settings", "Note #1"), tr("Should all settings be reset? This action cannot be undone!"));
    if (reply == QMessageBox::Yes)
    {
        QSettings("HKEY_CURRENT_USER\\SOFTWARE\\" + OTName, QSettings::NativeFormat).remove("");
        QMessageBox::information(this, tr("Reset settings", "Note #1"), tr("The programm will now restart."));

        misc.restart();
    }
}

/// \brief Creates a desktop shortcut
void wSettings::on_btnCreateDesktopShortcut_clicked()
{
    misc.createShortcut(qApp->applicationFilePath(), QDir().homePath() + QString("/Desktop/%1.lnk").arg(OTName), this);
}

/// \brief Changes and saves the theme (live)
void wSettings::on_cobxTheme_currentIndexChanged(int index)
{
    if (setupFinished)
    {
        if (index == 2)
            setStyleSheet(set.getStyleSheet("Darkeum"));
        else if (index == 1)
            setStyleSheet(set.getStyleSheet("Combinear"));
        else
            setStyleSheet("");

        set.write("main", "theme", index);

        setUnsaved(true);
    }
}

/// \brief Saves the language
void wSettings::on_cobxLanguage_currentIndexChanged(int index)
{
    if (setupFinished)
    {
        set.write("main", "language", index);
        //set.loadLanguage();
        setUnsaved(true);
    }
}

/// \brief Sets OMSI path
void wSettings::on_btnOmsiPath_clicked()
{
    QString mainDir = set.getOmsiPath(this, ui->ledOmsiPath->text());
    if ((mainDir != "") && (mainDir != ui->ledOmsiPath->text()))
    {
        ui->ledOmsiPath->setText(mainDir);
        setUnsaved(true);
    }

    set.write("main", "mainDir", mainDir);
}

/// \brief Saves the autosave duration
void wSettings::on_sbxAutosaveDuration_valueChanged(int arg1)
{
    if (setupFinished)
    {
        set.write("main", "autosaveDuration", arg1);
        setUnsaved(true);
    }
}

/// \brief Saves deletion confirmation
void wSettings::on_cbxConfirmDeletion_clicked()
{
    if (setupFinished)
    {
        set.write("main", "confirmDeletion", ui->cbxConfirmDeletion->isChecked());
        setUnsaved(true);
    }
}

/// \brief Saves the author's name
void wSettings::on_ledAuthor_textChanged(const QString &arg1)
{
    if (setupFinished)
    {
        set.write("main", "author", arg1);
        setUnsaved(true);
    }
}

/// \brief Turns autosave on / off
void wSettings::on_gbxAutosave_clicked()
{
    if (ui->gbxAutosave->isChecked())
        ui->sbxAutosaveDuration->setEnabled(true);
    else
        ui->sbxAutosaveDuration->setEnabled(false);

    set.write("main", "autosave", ui->gbxAutosave->isChecked());
    setUnsaved(true);
}

/// \brief Opens the logfile path in explorer
void wSettings::on_btnOpenLogfilePath_clicked()
{
    misc.showInExplorer(qApp->applicationDirPath() + "/logfile.txt");
}

/// \brief Shows a promotion to apply to a translator
void wSettings::on_btnMoreLanguages_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::information(this, tr("More languages", "Note #1"), tr("You want to have more languages to choose from?\nUnfortunately, there are no more languages at the moment. But if you know a language well, you are welcome to translate %1! Please contact the developer at the OMSI WebDisk, also if you have more questions.").arg(OTName), QMessageBox::Open | QMessageBox::Close);

    if (reply == QMessageBox::Open)
        QDesktopServices::openUrl(OTLinks::wipThread);
}

/// \brief Saves the update check
void wSettings::on_cobxAutoUpdateCheck_currentIndexChanged(int index)
{
    if (setupFinished)
    {
        set.write("main", "autoUpdateCheck", index);
        setUnsaved(true);
    }
}

/// \brief Saves the logfile mode
void wSettings::on_cobxLogfileMode_currentIndexChanged(int index)
{
    if (setupFinished)
    {
        int logfileMode;
        switch (index)
        {
            case 0: logfileMode = -1; break;
            case 1: logfileMode = 0; break;
            case 2: logfileMode = 1; break;
            case 3: logfileMode = 2; break;
            default: return; break;
        }

        set.write("main", "logfileMode", logfileMode);
        setUnsaved(true);
    }
}
