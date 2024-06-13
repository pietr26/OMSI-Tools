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

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshDiskUsage()));
    timer->start(10000);

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/en.svg"), OTStrings::langEn); // 0
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/de.svg"), OTStrings::langDe); // 1
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fr.svg"), OTStrings::langFr); // 2

    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/cs.svg"), OTStrings::langCs); // 3
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/fi.svg"), OTStrings::langFi); // 4
    ui->cobxLanguage->addItem(QIcon(":/rec/data/flags/ja.svg"), OTStrings::langJa); // 5

    // Disable languages:
    auto* model = qobject_cast<QStandardItemModel*>(ui->cobxLanguage->model());
    // model->item(0)->setEnabled(false); // en
    // model->item(1)->setEnabled(false); // de
    // model->item(2)->setEnabled(false); // fr
    // model->item(3)->setEnabled(false); // cs
    model->item(4)->setEnabled(false); // fi
    model->item(5)->setEnabled(false); // ja

    // cobxTheme:
    for (int i = 0; i < themes.count(); i++) ui->cobxTheme->addItem(themes[i].second);

    // lblDiskUsage
    ui->lblDiskUsageSize->setText(tr("Calculating..."));
    refreshDiskUsage();

    // sbAutosaveDuration
    ui->sbxAutosaveDuration->setSuffix(" s");

    // cobxLogfileMode
    QStringList logfileModes;
    logfileModes << tr("Off")/* -1 */ << tr("Standard (recommended)")/* 0 */ << "Debug"/* 1 */ << "Debug+"/* 2 */;
    ui->cobxLogfileMode->addItems(logfileModes);

    // btnDevTools
    ui->btnDevToolsPrefs->setVisible(set.devModeEnabled());

    loadPreferences();
    setupFinished = true;

    // Open direct modes:
    if (openDirect == "mainDirSelection") // Main directory selection
    {
        on_btnOmsiPath_clicked();
        QTimer::singleShot(0, this, SLOT(close()));
    }
    else if (openDirect == "devTools") // devTools prefs
    {
        ui->lwgSections->setCurrentRow(0);
    }
    else if (openDirect == "wVerifyMap") // wVerifyMap prefs
    {
        ui->lwgSections->setCurrentRow(1);
    }
    else if (openDirect == "wFonts") // wFonts prefs
    {
        ui->lwgSections->setCurrentRow(2);
    }
    else ui->lwgSections->setCurrentRow(0);

    on_lwgSections_itemClicked(new QListWidgetItem());

    qInfo().noquote() << objectName() + " started";
}

wPreferences::~wPreferences()
{
    delete ui;
}

void wPreferences::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    qApp->setStyle(QStyleFactory::create(set.read("main", "theme").toString()));
}

void wPreferences::loadPreferences()
{
    // GENERAL
    {
        // General
        {
            // Language
            int languageIndex = set.read("main", "language").toInt();
            ui->cobxLanguage->setCurrentIndex(languageIndex);

            // Theme
            QString theme = set.read("main", "theme").toString();

            for (int i = 0; i < ui->cobxTheme->count(); i++)
            {
                if (themes[i].first == theme)
                {
                    ui->cobxTheme->setCurrentIndex(i);
                    break;
                }
            }

            // Author
            ui->ledAuthor->setText(set.read("main", "author").toString());

            // Omsi Path
            ui->ledOmsiPath->setText(set.read("main", "mainDir").toString());

            // Logfile mode
            ui->cobxLogfileMode->setCurrentIndex(set.read("main", "logfileMode").toInt() + 1);

            // News section
            ui->cbxShowNews->setChecked(set.read("wStart", "messagesVisible").toBool());

            // DiscordGameSDK
            ui->cbxDiscordGameSDK->setChecked(set.read("main", "useDiscordGameSDK").toBool());
        };

        // Backup
        {
            // Auto save enabled
            ui->cbxBackupEnabled->setChecked(set.read("main", "autosave").toBool());

            // Auto save duration
            ui->sbxAutosaveDuration->setValue(set.read("main", "autosaveDuration").toInt());
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

    // FONT CREATION
    {
        // keep pixel row
        ui->cbxKeepPixelRow->setChecked(set.read("wFonts", "keepPixelRow").toBool());
    };

    // DEVTOOLS
    {
        // BugDoc
        {
            // Screenshot scale
            ui->sbxBugDocScreenshotScale->setValue(set.read("wBugDoc", "screenshotScale").toInt() == 0 ? 2 : set.read("wBugDoc", "screenshotScale").toInt());
        };
    };

    setWindowModified(false);
}

void wPreferences::savePreferences()
{
    // GENERAL
    {
        // General
        {
            // Language
            set.write("main", "language", ui->cobxLanguage->currentIndex());

            // Theme
            set.write("main", "theme", themes[ui->cobxTheme->currentIndex()].first);

            // Author
            set.write("main", "author", ui->ledAuthor->text());

            // Omsi Path
            set.write("main", "mainDir", ui->ledOmsiPath->text());

            // Logfile mode
            set.write("main", "logfileMode", ui->cobxLogfileMode->currentIndex() - 1);

            // News section
            set.write("wStart", "messagesVisible", ui->cbxShowNews->isChecked());

            // News section
            set.write("main", "useDiscordGameSDK", ui->cbxDiscordGameSDK->isChecked());
        };

        // Backup
        {
            // Auto save enabled
            set.write("main", "autosave", ui->cbxBackupEnabled->isChecked());

            // Auto save duration
            set.write("main", "autosaveDuration", ui->sbxAutosaveDuration->value());
        };
    };


    // MAP VERFICATION
    {
        // adv verifying
        set.write("wVerifyMap", "advVerifying", ui->cbxAdvancedVerifying->isChecked());

        // map textures
        set.write("wVerifyMap", "onlyMapTextures", ui->cbxOnlyMapTextures->isChecked());
    };

    // FONT CREATION
    {
        // keep pixel row
        set.write("wFonts", "keepPixelRow", ui->cbxKeepPixelRow->isChecked());
    };

    // DEVTOOLS
    {
        // BugDoc
        {
            // Screenshot scale
            set.write("wBugDoc", "screenshotScale", ui->sbxBugDocScreenshotScale->value());
        };
    };

    setWindowModified(false);
}

void wPreferences::modified()
{
    setWindowModified(true);
}

void wPreferences::refreshDiskUsage()
{
    ui->lblDiskUsageSize->setText(dUs.formatSize("backup"));
}

void wPreferences::on_btnClose_clicked()
{
    on_actionClose_triggered();
}

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

void wPreferences::on_btnOpenBackupFolder_clicked()
{
    QDir().mkdir("backup");
    QString path = QFileInfo("./backup").absolutePath();
    path += "/backup";

    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void wPreferences::on_btnCheckForUpdates_clicked()
{
    QPair<int, QString> update = updater->getUpdateInformation();

    if (update.first == -2)
        QMessageBox::information(this, tr("Maintenance"), OTStrings::serverMaintenance());
    else if (update.first == -1)
        QMessageBox::warning(this, tr("Failed"), tr("Could not check for updates. Please check your internet connection and try again.") + "\n(-1)");
    else if (update.first == 0)
        QMessageBox::information(this, tr("Finshed"), tr("No updates available."));
    else if (update.first > 0)
    {
        hide();
        WRELEASENOTES = new wReleaseNotes(this, true, update.second, (update.first == 2) ? true : false);
        WRELEASENOTES->setWindowModality(Qt::ApplicationModal);
        WRELEASENOTES->show();
    }
}

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

void wPreferences::on_btnOmsiPath_clicked()
{
    QString mainDir = set.getOmsiPath(this, true, ui->ledOmsiPath->text());
    ui->ledOmsiPath->setText(mainDir);
    modified();
}

void wPreferences::on_btnOpenLogfilePath_clicked()
{
    fop.showInExplorer(qApp->applicationDirPath() + "/logfile.txt");
}

void wPreferences::on_btnMoreLanguages_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::information(this, tr("More languages"), tr("You want to have more languages to choose from?\nUnfortunately, there are no more languages at the moment. But if you know a language well, you are welcome to translate %1! Please contact the developer at the OMSI WebDisk, also if you have more questions.").arg(OTInformation::name), QMessageBox::Open | QMessageBox::Cancel);

    if (reply == QMessageBox::Open)
        QDesktopServices::openUrl(OTLinks::showroom);
}

void wPreferences::on_cobxAutoUpdateCheck_currentIndexChanged(int index) { Q_UNUSED(index); modified(); }

void wPreferences::on_cobxLogfileMode_currentIndexChanged(int index) { Q_UNUSED(index); if (setupFinished) { modified(); needRestart = true; } }

void wPreferences::on_sbxAutosaveDuration_valueChanged(int arg1) { Q_UNUSED(arg1); modified(); }

void wPreferences::on_ledAuthor_textChanged(const QString &arg1) { Q_UNUSED(arg1); modified(); }

void wPreferences::on_cbxBackupEnabled_clicked(bool checked) { Q_UNUSED(checked); modified(); }

void wPreferences::on_cbxAdvancedVerifying_stateChanged(int arg1) { modified(); ui->cbxOnlyMapTextures->setEnabled(arg1); }

void wPreferences::on_cbxOnlyMapTextures_stateChanged(int arg1) { Q_UNUSED(arg1); modified(); }

void wPreferences::on_cbxShowNews_stateChanged(int arg1) { Q_UNUSED(arg1); if (setupFinished) { modified(); needRestart = true; } }

void wPreferences::on_cobxLanguage_currentIndexChanged(int index) { Q_UNUSED(index); modified(); if (setupFinished) { needRestart = true; } }

void wPreferences::on_cbxKeepPixelRow_stateChanged(int arg1) { Q_UNUSED(arg1); modified(); }

void wPreferences::on_cbxDiscordGameSDK_stateChanged(int arg1) { Q_UNUSED(arg1); if (setupFinished) { modified(); needRestart = true; } }

void wPreferences::reloadThemePreview()
{
    qApp->setStyle(QStyleFactory::create(themes[ui->cobxTheme->currentIndex()].first));
}

void wPreferences::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::preferences);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

void wPreferences::on_btnSave_clicked()
{
    savePreferences();

    if (needRestart) {
        QMessageBox::information(this, tr("Restart required"), tr("To fully apply all changed preferences, the application requires a restart."));
    }

    needRestart = false;

    close();
}

void wPreferences::on_btnDevToolsPrefs_clicked()
{
    ui->lwgSections->clearSelection();
    ui->stwPreferences->setCurrentIndex(0);
    ui->lblCurrentSection->setText("DevTools");
}

void wPreferences::on_lwgSections_itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    ui->stwPreferences->setCurrentIndex(ui->lwgSections->currentRow() + 1);
    ui->lblCurrentSection->setText(ui->lwgSections->currentItem()->text());
}

void wPreferences::on_actionClose_triggered()
{
    if (isWindowModified())
    {
        if (msg.unsavedContentLeaveYesNo(this) == 1)
            close();
    }
    else close();
}

void wPreferences::on_cobxTheme_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    if (setupFinished) reloadThemePreview();
}
