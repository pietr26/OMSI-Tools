#include "wpreferences.h"
#include "ui_wpreferences.h"

wPreferences::wPreferences(QWidget *parent, QString openDirect) :
    QMainWindow(parent),
    ui(new Ui::wPreferences)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    resize(misc.sizeWindow(0.45, 0.6));
    qDebug() << "UI set";

    setWindowTitle("[*] " + OTInformation::name + " - " + tr("preferences"));

    ui->btnUseCustomTheme->setVisible(false);
    ui->btnRestart->setVisible(false);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshDiskUsage()));
    timer->start(10000);


    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/en.svg"), OTStrings::langEn); // 0
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/de.svg"), OTStrings::langDe); // 1
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fr.svg"), OTStrings::langFr); // 2

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/it.svg"), OTStrings::langIt); // 3

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cs.svg"), OTStrings::langCs); // 4

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/et.svg"), OTStrings::langEt); // 5
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/eo.svg"), OTStrings::langEo); // 6
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fi.svg"), OTStrings::langFi); // 7
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/is.svg"), OTStrings::langIs); // 8
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/ja.svg"), OTStrings::langJa); // 9
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cy.svg"), OTStrings::langCy); // 10

    // Disable languages:
    auto* model = qobject_cast<QStandardItemModel*>(ui->cobxLanguage->model());
    // model->item(0)->setEnabled(false); // en
    // model->item(1)->setEnabled(false); // de
    // model->item(2)->setEnabled(false); // fr
    model->item(3)->setEnabled(false); // it
    // model->item(4)->setEnabled(false); // cs
    model->item(5)->setEnabled(false); // et
    model->item(6)->setEnabled(false); // eo
    model->item(7)->setEnabled(false); // fi
    model->item(8)->setEnabled(false); // is
    model->item(9)->setEnabled(false); // ja
    model->item(10)->setEnabled(false); // cy

    // cobxTheme:
    ui->cobxTheme->addItem(tr("Standard"));
    ui->cobxTheme->addItem(tr("Modern light"));
    ui->cobxTheme->addItem(tr("Modern dark"));

    // lblDiskUsage
    ui->lblDiskUsageSize->setText(tr("Calculating..."));
    refreshDiskUsage();

    // sbAutosaveDuration
    ui->sbxAutosaveDuration->setSuffix(" " + tr("s", "Short version of 'seconds'"));

    // cobxLogfileMode
    QStringList logfileModes;
    logfileModes << tr("Off")/* -1 */ << tr("Standard")/* 0 */ << "Debug"/* 1 */ << "Debug+"/* 2 */;
    ui->cobxLogfileMode->addItems(logfileModes);

    // cobxAutoUpdateCheck
    QStringList updateModes;
    updateModes << tr("Off")/* 0 */ << tr("On start")/* 1 */ << tr("Daily")/* 2 */ << tr("Weekly")/* 3 */ << tr("Monthly")/* 4 */;
    ui->cobxAutoUpdateCheck->addItems(updateModes);

    loadSettings();
    isFirstSetup = false;

    // Open direct modes:
    if (openDirect == "mainDirSelection") // Main directory selection
    {
        on_btnOmsiPath_clicked();
        QTimer::singleShot(0, this, SLOT(close()));
    }
    else if (openDirect == "wVerifyMap") // wVerifyMap prefs
        ui->lwgSections->setCurrentRow(1);

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

void wPreferences::loadSettings()
{
    // GENERAL
    {
        // General
        {
            // Language
            int languageIndex = set.read("main", "language").toInt();
            ui->cobxLanguage->setCurrentIndex(languageIndex);

            // Author
            ui->ledAuthor->setText(set.read("main", "author").toString());

            // Omsi Path
            ui->ledOmsiPath->setText(set.read("main", "mainDir").toString());
        };

        // Theme
        {
            // Theme
            useStandardTheme = set.read("main\\themeData", "useStandardTheme").toBool();
            ui->gbxThemeAdvanced->setEnabled(!useStandardTheme);
            reloadThemePreview();



            ui->btnUseCustomTheme->setVisible(useStandardTheme);
        };

        // Backup
        {
            // Auto save enabled
            ui->cbxBackupEnabled->setChecked(set.read("main", "autosave").toBool());

            // Auto save duration
            ui->sbxAutosaveDuration->setValue(set.read("main", "autosaveDuration").toInt());
        };

        // Misc
        {
            // Logfile mode
            ui->cobxLogfileMode->setCurrentIndex(set.read("main", "logfileMode").toInt() + 1);

            // Auto update check
            ui->cobxAutoUpdateCheck->setCurrentIndex(set.read("main", "autoUpdateCheck").toInt());
        };
    };


    // MAP VERFICATION
    {
        // adv verifying
        ui->cbxAdvancedVerifying->setChecked(set.read("wVerifyMap", "advVerifying").toBool());

        // map textures
        ui->cbxOnlyMapTextures->setChecked(set.read("wVerifyMap", "onlyMapTextures").toBool());
        ui->cbxOnlyMapTextures->setEnabled(ui->cbxAdvancedVerifying->isChecked());
    };

    setWindowModified(false);
}

void wPreferences::saveSettings()
{
    // GENERAL
    {
        // General
        {
            // Language
            set.write("main", "language", ui->cobxLanguage->currentIndex());

            // Author
            set.write("main", "author", ui->ledAuthor->text());

            // Omsi Path
            set.write("main", "mainDir", ui->ledOmsiPath->text());
        };

        // Theme
        {
            // Theme data
            set.remove("main\\themeData", "");
            set.write("main\\themeData", "Main", tcMain);
            set.write("main\\themeData", "Dis", tcDis);
            set.write("main\\themeData", "DisD", tcDisD);
            set.write("main\\themeData", "Acc1", tcAcc1);
            set.write("main\\themeData", "Acc2", tcAcc2);
            set.write("main\\themeData", "Acc3", tcAcc3);
            set.write("main\\themeData", "Button", tcButton);

            set.write("main\\themeData", "useStandardTheme", useStandardTheme);
        };

        // Backup
        {
            // Auto save enabled
            set.write("main", "autosave", ui->cbxBackupEnabled->isChecked());

            // Auto save duration
            set.write("main", "autosaveDuration", ui->sbxAutosaveDuration->value());
        };

        // Misc
        {
            // Logfile mode
            set.write("main", "logfileMode", ui->cobxLogfileMode->currentIndex() - 1);

            // Auto update check
            set.write("main", "autoUpdateCheck", ui->cobxAutoUpdateCheck->currentIndex());
        };
    };


    // MAP VERFICATION
    {
        // adv verifying
        set.write("wVerifyMap", "advVerifying", ui->cbxAdvancedVerifying->isChecked());

        // map textures
        set.write("wVerifyMap", "onlyMapTextures", ui->cbxOnlyMapTextures->isChecked());
    };

    setWindowModified(false);
}

void wPreferences::modified()
{
    setWindowModified(true);
}

/// Refreshes disk usage for backup folder
void wPreferences::refreshDiskUsage()
{
    ui->lblDiskUsageSize->setText(dUs.formatSize("backup"));
}

/// Closes the application
void wPreferences::on_btnClose_clicked()
{
    if (isWindowModified())
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Unsaved changes"), tr("There are unsaved changes. Close anyway?"));
        if (reply == QMessageBox::Yes) close();
    }
    else close();
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
void wPreferences::on_cobxLanguage_currentIndexChanged(int index) { Q_UNUSED(index); modified(); needRestart = true; }

/// Sets OMSI path
void wPreferences::on_btnOmsiPath_clicked()
{
    QString mainDir = set.getOmsiPath(this, true, ui->ledOmsiPath->text());
    ui->ledOmsiPath->setText(mainDir);
    modified();
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

void wPreferences::on_cobxAutoUpdateCheck_currentIndexChanged(int index) { Q_UNUSED(index); modified(); }

void wPreferences::on_cobxLogfileMode_currentIndexChanged(int index) { Q_UNUSED(index); modified(); needRestart = true; }

void wPreferences::on_sbxAutosaveDuration_valueChanged(int arg1) { Q_UNUSED(arg1); modified(); }

void wPreferences::on_ledAuthor_textChanged(const QString &arg1) { Q_UNUSED(arg1); modified(); }

void wPreferences::on_cbxBackupEnabled_clicked(bool checked) { Q_UNUSED(checked); modified(); }

void wPreferences::on_cbxAdvancedVerifying_stateChanged(int arg1) { Q_UNUSED(arg1); modified(); }

void wPreferences::on_cbxOnlyMapTextures_stateChanged(int arg1) { Q_UNUSED(arg1); modified(); }

/// Reloads theme preview
void wPreferences::reloadThemePreview()
{
    if (isFirstSetup) tcMain = set.read("main\\themeData", "Main").toString();
    ui->lblThemeMain->setStyleSheet(QString("color: %1").arg(tcMain));
    if (tcMain.isEmpty()) ui->lblThemeMain->setStyleSheet("");

    if (isFirstSetup) tcDis = set.read("main\\themeData", "Dis").toString();
    ui->lblThemeDis->setStyleSheet(QString("color: %1").arg(tcDis));
    if (tcDis.isEmpty()) ui->lblThemeDis->setStyleSheet("");

    if (isFirstSetup) tcDisD = set.read("main\\themeData", "DisD").toString();
    ui->lblThemeDisD->setStyleSheet(QString("color: %1").arg(tcDisD));
    if (tcDisD.isEmpty()) ui->lblThemeDisD->setStyleSheet("");

    if (isFirstSetup) tcAcc1 = set.read("main\\themeData", "Acc1").toString();
    ui->lblThemeAcc1->setStyleSheet(QString("color: %1").arg(tcAcc1));
    if (tcAcc1.isEmpty()) ui->lblThemeAcc1->setStyleSheet("");

    if (isFirstSetup) tcAcc2 = set.read("main\\themeData", "Acc2").toString();
    ui->lblThemeAcc2->setStyleSheet(QString("color: %1").arg(tcAcc2));
    if (tcAcc2.isEmpty()) ui->lblThemeAcc2->setStyleSheet("");

    if (isFirstSetup) tcAcc3 = set.read("main\\themeData", "Acc3").toString();
    ui->lblThemeAcc3->setStyleSheet(QString("color: %1").arg(tcAcc3));
    if (tcAcc3.isEmpty()) ui->lblThemeAcc3->setStyleSheet("");

    if (isFirstSetup) tcButton = set.read("main\\themeData", "Button").toString();
    ui->lblThemeButton->setStyleSheet(QString("color: %1").arg(tcButton));
    if (tcButton.isEmpty()) ui->lblThemeButton->setStyleSheet("");

    setStyleSheet(set.getStyleSheet(tcMain, tcDis, tcDisD, tcAcc1, tcAcc2, tcAcc3, tcButton, useStandardTheme));
}

/// Opens color dialog for main color
void wPreferences::on_btnThemeMain_clicked()
{
    QColor color = QColorDialog::getColor(QColor(set.read("main\\themeData", "Main").toString()), this, tr("Select main color")).name();
    if (color.isValid()) tcMain = color.name();
    reloadThemePreview();
    needRestart = true;
}

/// Opens color dialog for disables color
void wPreferences::on_btnThemeDis_clicked()
{
    QColor color = QColorDialog::getColor(QColor(set.read("main\\themeData", "Dis").toString()), this, tr("Select disabled color"));
    if (color.isValid()) tcDis = color.name();
    reloadThemePreview();
    needRestart = true;
}

/// Opens color dialog for disables (darker) color
void wPreferences::on_btnThemeDisD_clicked()
{
    QColor color = QColorDialog::getColor(QColor(set.read("main\\themeData", "DisD").toString()), this, tr("Select disabled background color"));
    if (color.isValid()) tcDisD = color.name();
    reloadThemePreview();
    needRestart = true;
}

/// Opens color dialog for accent 1 color
void wPreferences::on_btnThemeAcc1_clicked()
{
    QColor color = QColorDialog::getColor(QColor(set.read("main\\themeData", "Acc1").toString()), this, tr("Select accent color"));
    if (color.isValid()) tcAcc1 = color.name();
    reloadThemePreview();
    needRestart = true;
}

/// Opens color dialog for accent 2 color
void wPreferences::on_btnThemeAcc2_clicked()
{
    QColor color = QColorDialog::getColor(QColor(set.read("main\\themeData", "Acc2").toString()), this, tr("Select main accent color"));
    if (color.isValid()) tcAcc2 = color.name();
    reloadThemePreview();
    needRestart = true;
}

/// Opens color dialog for accent 3 color
void wPreferences::on_btnThemeAcc3_clicked()
{
    QColor color = QColorDialog::getColor(QColor(set.read("main\\themeData", "Acc3").toString()), this, tr("Select font color"));
    if (color.isValid()) tcAcc3 = color.name();
    reloadThemePreview();
    needRestart = true;
}

/// Opens color dialog for button color
void wPreferences::on_btnThemeButton_clicked()
{
    QColor color = QColorDialog::getColor(set.read("main\\themeData", "Button").toString(), this, tr("Select button color"));
    if (color.isValid()) tcButton = color.name();
    reloadThemePreview();
    needRestart = true;
}

/// Loads a default theme
void wPreferences::on_btnLoadTheme_clicked()
{
    set.getDefaultThemeData(ui->cobxTheme->currentIndex(), tcMain, tcDis, tcDisD, tcAcc1, tcAcc2, tcAcc3, tcButton, useStandardTheme);

    ui->gbxThemeAdvanced->setEnabled(!useStandardTheme);
    ui->btnUseCustomTheme->setVisible(useStandardTheme);
    reloadThemePreview();
    needRestart = true;

    modified();
}

/// Opens help dialog
void wPreferences::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::preferences);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

void wPreferences::on_lwgSections_currentRowChanged(int currentRow)
{
    ui->stwPreferences->setCurrentIndex(currentRow);
    ui->lblCurrentSection->setText(ui->lwgSections->currentItem()->text());
}

void wPreferences::on_btnSave_clicked()
{
    saveSettings();

    if (needRestart) {
        QMessageBox::information(this, tr("Restart required"), tr("To fully apply all changed preferences, the application requires a restart."));
        ui->btnRestart->setVisible(true);
    }
}


void wPreferences::on_btnUseCustomTheme_clicked()
{
    ui->gbxThemeAdvanced->setEnabled(true);
    useStandardTheme = false;
    reloadThemePreview();
    ui->btnUseCustomTheme->setVisible(false);
}

