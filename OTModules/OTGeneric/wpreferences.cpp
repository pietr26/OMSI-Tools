#include "wpreferences.h"
#include "ui_wpreferences.h"

wPreferences::wPreferences(QWidget *parent, QString openDirect) :
    QMainWindow(parent),
    ui(new Ui::wPreferences)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    resize(misc.sizeWindow(0.3, 0.4));
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - " + tr("preferences"));

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshDiskUsage()));
    timer->start(10000);

    // cobxLanguage
    int languageIndex = set.read("main", "language").toInt();
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/en.svg"), OTStrings::langEn); // 0
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/de.svg"), OTStrings::langDe); // 1
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fr.svg"), OTStrings::langFr); // 2
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/it.svg"), OTStrings::langIt); // 3
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cz.svg"), OTStrings::langCz); // 4

    // Disable 'it'
    auto* model3 = qobject_cast<QStandardItemModel*>(ui->cobxLanguage->model());
    model3->item(3)->setEnabled(false);

    ui->cobxLanguage->setCurrentIndex(languageIndex);

    // cobxTheme:
    ui->cobxTheme->addItem(tr("Standard"));
    ui->cobxTheme->addItem(tr("Modern light"));
    ui->cobxTheme->addItem(tr("Modern dark"));

    reloadThemePreview();
    ui->gbxThemeAdvanced->setEnabled(!set.read("main\\themeData", "useStandardTheme").toBool());

    // cbxAutoSave
    if (set.read("main", "autosave") == "true")
        ui->cbxBackupEnabled->setChecked(true);
    else if (set.read("main", "autosave") == "false")
        ui->cbxBackupEnabled->setChecked(false);

    // lblDiskUsage
    ui->lblDiskUsageSize->setText(tr("Calculating..."));
    refreshDiskUsage();

    // sbxAutosaveDuration
    QString autosaveDuration = set.read("main", "autosaveDuration").toString();
    if (!autosaveDuration.isEmpty())
        ui->sbxAutosaveDuration->setValue(set.read("main", "autosaveDuration").toInt());

    ui->sbxAutosaveDuration->setSuffix(" s");

    // ledAuthor
    ui->ledAuthor->setText(set.read("main", "author").toString());

    // ledOmsiPath
    ui->ledOmsiPath->setText(set.read("main", "mainDir").toString());

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

    setupFinished = true;

    setUnsaved(false);

    ui->twgPreferences->setTabVisible(4, false);

    // Open direct modes:
    // Main directory selection
    if (openDirect == "mainDirSelection")
    {
        on_btnOmsiPath_clicked();
        QTimer::singleShot(0, this, SLOT(close()));
    }
    // wVerifyMap prefs
    else if (openDirect == "wVerifyMap")
    {
        ui->twgPreferences->setTabVisible(4, true);
        ui->twgPreferences->setCurrentIndex(4);
        qApp->processEvents();

        QFont bold;
        bold.setWeight(QFont::Bold);
        ui->gbxMapVerification->setFont(bold);

        if (!set.read("wVerifyMap", "advVerifying").isValid())
            set.write("wVerifyMap", "advVerifying", false);
        else
            ui->cbxAdvancedVerifying->setChecked(set.read("wVerifyMap", "advVerifying").toBool());

        if (!set.read("wVerifyMap", "onlyMapTextures").isValid())
            set.write("wVerifyMap", "onlyMapTextures", false);
        else
            ui->cbxOnlyMapTextures->setChecked(set.read("wVerifyMap", "onlyMapTextures").toBool());

        ui->cbxOnlyMapTextures->setEnabled(ui->cbxAdvancedVerifying->isChecked());
    }

    qInfo().noquote() << objectName() + " started";
}

wPreferences::~wPreferences()
{
    delete ui;
}

void wPreferences::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
}

/// Sets unsaved
void wPreferences::setUnsaved(bool visible)
{
    if (visible)
        ui->statusbar->showMessage(tr("Restart to apply all preferences."));
    else
        ui->statusbar->showMessage("", 10);
}

/// Refreshes disk usage for backup folder
void wPreferences::refreshDiskUsage()
{
    ui->lblDiskUsageSize->setText(dUs.formatSize("backup"));
}

/// Closes the application
void wPreferences::on_btnClose_clicked()
{
    close();
}

/// Deletes the backup folder
void wPreferences::on_btnDeleteAllBackups_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Delete all backups"), tr("Should all backups be deleted? They will be moved to the recycle bin."));

    if (reply == QMessageBox::Yes)
    {
        qDebug() << "Move to trash:" << QFile::moveToTrash("backup");
        ui->btnDeleteAllBackups->setEnabled(false);
        refreshDiskUsage();
        qInfo().noquote() << "Deleted backups";
    }
}

/// Opens the backup folder
void wPreferences::on_btnOpenBackupFolder_clicked()
{
    QDir().mkdir("backup");
    QString path = QFileInfo("./backup").absolutePath();
    path += "/backup";

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

/// Saves and restarts the application
void wPreferences::on_btnRestart_clicked()
{
//    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Restart now?"), tr("Should the application be restarted now? Any unsaved content will be discarded."));

//    if (reply == QMessageBox::Yes)
        misc.restart();
}

/// Checks for updates
void wPreferences::on_btnCheckForUpdates_clicked()
{
    QStringList update = misc.getUpdateInformation();

    if (update.at(0) == "503")
        QMessageBox::information(this, tr("Maintenance"), OTStrings::serverMaintenance());
    else if (update.at(0) == "noUpdates")
        QMessageBox::information(this, tr("Finshed"), tr("No updates available."));
    else if (update.at(0) != "false")
    {
        hide();
        WRELEASENOTES = new wReleaseNotes(this, true, update.at(1));
        WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
        WRELEASENOTES->show();
    }
}

/// Resets the prefs
void wPreferences::on_btnResetPreferences_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Reset preferences"), tr("Should all preferences be reset? This action cannot be undone! Any preferences will be deleted. However, files such as backups are not affected."), QMessageBox::Reset | QMessageBox::Cancel);
    if (reply == QMessageBox::Reset)
    {
        set.removeAll();
        QMessageBox::information(this, tr("Reset preferences"), tr("The application will now restart."));
        misc.restart();
    }
}

/// Creates a desktop shortcut
void wPreferences::on_btnCreateDesktopShortcut_clicked()
{
    fop.createShortcut(qApp->applicationFilePath(), QDir().homePath() + QString("/Desktop/%1.lnk").arg(OTInformation::name), this);
}

/// Saves the language
void wPreferences::on_cobxLanguage_currentIndexChanged(int index)
{
    if (setupFinished)
    {
        set.write("main", "language", index);
        setUnsaved(true);
    }
}

/// Sets OMSI path
void wPreferences::on_btnOmsiPath_clicked()
{
    QString mainDir = set.getOmsiPath(this, true, ui->ledOmsiPath->text());
    ui->ledOmsiPath->setText(mainDir);
    set.write("main", "mainDir", mainDir);
    setUnsaved(true);
}

/// Saves the autosave duration
void wPreferences::on_sbxAutosaveDuration_valueChanged(int arg1)
{
    if (setupFinished)
    {
        set.write("main", "autosaveDuration", arg1);
        setUnsaved(true);
    }
}

/// Saves the author's name
void wPreferences::on_ledAuthor_textChanged(const QString &arg1)
{
    if (setupFinished)
    {
        set.write("main", "author", arg1);
        setUnsaved(true);
    }
}

/// Turns backup on / off
void wPreferences::on_cbxBackupEnabled_clicked(bool checked)
{
    set.write("main", "autosave", checked);
    setUnsaved(true);
}

/// Opens the logfile path in explorer
void wPreferences::on_btnOpenLogfilePath_clicked()
{
    fop.showInExplorer(qApp->applicationDirPath() + "/logfile.txt");
}

/// Shows a promotion to apply to a translator
void wPreferences::on_btnMoreLanguages_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::information(this, tr("More languages"), tr("You want to have more languages to choose from?\nUnfortunately, there are no more languages at the moment. But if you know a language well, you are welcome to translate %1! Please contact the developer at the OMSI WebDisk, also if you have more questions.").arg(OTInformation::name), QMessageBox::Open | QMessageBox::Cancel);

    if (reply == QMessageBox::Open)
        QDesktopServices::openUrl(OTLinks::showroom);
}

/// Saves the update check
void wPreferences::on_cobxAutoUpdateCheck_currentIndexChanged(int index)
{
    if (setupFinished)
    {
        set.write("main", "autoUpdateCheck", index);
        setUnsaved(true);
    }
}

/// Saves the logfile mode
void wPreferences::on_cobxLogfileMode_currentIndexChanged(int index)
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

/// Reloads theme preview
void wPreferences::reloadThemePreview()
{
    if (!set.read("main\\themeData", "Main").toString().isEmpty())
        ui->lblThemeMain->setStyleSheet(QString("color: %1").arg(set.read("main\\themeData", "Main").toString()));
    else
        ui->lblThemeMain->setStyleSheet("");

    if (!set.read("main\\themeData", "MainSC").toString().isEmpty())
        ui->lblThemeMainSC->setStyleSheet(QString("color: %1").arg(set.read("main\\themeData", "MainSC").toString()));
    else
        ui->lblThemeMainSC->setStyleSheet("");

    if (!set.read("main\\themeData", "Dis").toString().isEmpty())
        ui->lblThemeDis->setStyleSheet(QString("color: %1").arg(set.read("main\\themeData", "Dis").toString()));
    else
        ui->lblThemeDis->setStyleSheet("");

    if (!set.read("main\\themeData", "DisD").toString().isEmpty())
        ui->lblThemeDisD->setStyleSheet(QString("color: %1").arg(set.read("main\\themeData", "DisD").toString()));
    else
        ui->lblThemeDisD->setStyleSheet("");

    if (!set.read("main\\themeData", "Acc1").toString().isEmpty())
        ui->lblThemeAcc1->setStyleSheet(QString("color: %1").arg(set.read("main\\themeData", "Acc1").toString()));
    else
        ui->lblThemeAcc1->setStyleSheet("");

    if (!set.read("main\\themeData", "Acc2").toString().isEmpty())
        ui->lblThemeAcc2->setStyleSheet(QString("color: %1").arg(set.read("main\\themeData", "Acc2").toString()));
    else
        ui->lblThemeAcc2->setStyleSheet("");

    if (!set.read("main\\themeData", "Acc3").toString().isEmpty())
        ui->lblThemeAcc3->setStyleSheet(QString("color: %1").arg(set.read("main\\themeData", "Acc3").toString()));
    else
        ui->lblThemeAcc3->setStyleSheet("");

    if (!set.read("main\\themeData", "Button").toString().isEmpty())
        ui->lblThemeButton->setStyleSheet(QString("color: %1").arg(set.read("main\\themeData", "Button").toString()));
    else
        ui->lblThemeButton->setStyleSheet("");

    setStyleSheet(set.getStyleSheet());
}

/// Opens color dialog for main color
void wPreferences::on_btnThemeMain_clicked()
{
    QString hex = QColorDialog::getColor(QColor(set.read("main\\themeData", "Main").toString()), this, tr("Select main color")).name();
    set.write("main\\themeData", "Main", hex);
    reloadThemePreview();
}

/// Opens color dialog for Main (simple contrast) color
void wPreferences::on_btnThemeMainSC_clicked()
{
    QString hex = QColorDialog::getColor(QColor(set.read("main\\themeData", "MainSC").toString()), this, tr("Select border color")).name();
    set.write("main\\themeData", "MainSC", hex);
    reloadThemePreview();

}

/// Opens color dialog for disables color
void wPreferences::on_btnThemeDis_clicked()
{
    QString hex = QColorDialog::getColor(QColor(set.read("main\\themeData", "Dis").toString()), this, tr("Select disabled color")).name();
    set.write("main\\themeData", "Dis", hex);
    reloadThemePreview();

}

/// Opens color dialog for disables (darker) color
void wPreferences::on_btnThemeDisD_clicked()
{
    QString hex = QColorDialog::getColor(QColor(set.read("main\\themeData", "DisD").toString()), this, tr("Select disabled background color")).name();
    set.write("main\\themeData", "DisD", hex);
    reloadThemePreview();
}

/// Opens color dialog for accent 1 color
void wPreferences::on_btnThemeAcc1_clicked()
{
    QString hex = QColorDialog::getColor(QColor(set.read("main\\themeData", "Acc1").toString()), this, tr("Select accent color")).name();
    set.write("main\\themeData", "Acc1", hex);
    reloadThemePreview();

}

/// Opens color dialog for accent 2 color
void wPreferences::on_btnThemeAcc2_clicked()
{
    QString hex = QColorDialog::getColor(QColor(set.read("main\\themeData", "Acc2").toString()), this, tr("Select main accent color")).name();
    set.write("main\\themeData", "Acc2", hex);
    reloadThemePreview();
}

/// Opens color dialog for accent 3 color
void wPreferences::on_btnThemeAcc3_clicked()
{
    QString hex = QColorDialog::getColor(QColor(set.read("main\\themeData", "Acc3").toString()), this, tr("Select font color")).name();
    set.write("main\\themeData", "Acc3", hex);
    reloadThemePreview();
}

/// Opens color dialog for button color
void wPreferences::on_btnThemeButton_clicked()
{
    QString hex = QColorDialog::getColor(set.read("main\\themeData", "Button").toString(), this, tr("Select button color")).name();
    set.write("main\\themeData", "Button", hex);
    reloadThemePreview();
}

/// Loads a default theme
void wPreferences::on_btnLoadTheme_clicked()
{
    set.setDefaultTheme(ui->cobxTheme->currentIndex());
    reloadThemePreview();
    setUnsaved(true);

    ui->gbxThemeAdvanced->setEnabled(!set.read("main\\themeData", "useStandardTheme").toBool());

    if (set.read("main\\themeData", "useStandardTheme").toBool()) setStyleSheet("");
}

/// Opens help dialog
void wPreferences::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::wiki::preferences);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

/// Saves advanced verifying setting
void wPreferences::on_cbxAdvancedVerifying_stateChanged(int arg1)
{
    if (arg1 == 2)
    {
        set.write("wVerifyMap", "advVerifying", true);
        ui->cbxOnlyMapTextures->setEnabled(true);
    }
    else
    {
        set.write("wVerifyMap", "advVerifying", false);
        ui->cbxOnlyMapTextures->setEnabled(false);
    }
}

/// Saves only map textures setting
void wPreferences::on_cbxOnlyMapTextures_stateChanged(int arg1)
{
    if (arg1 == 2)
        set.write("wVerifyMap", "onlyMapTextures", true);
    else
        set.write("wVerifyMap", "onlyMapTextures", false);
}
