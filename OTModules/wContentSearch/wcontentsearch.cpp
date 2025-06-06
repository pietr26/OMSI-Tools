#include "wcontentsearch.h"
#include "ui_wcontentsearch.h"

wContentSearch::wContentSearch(QWidget *parent, QStringList paths) :
    QMainWindow(parent),
    ui(new Ui::wContentSearch)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    resize(misc.sizeWindow(0.6, 0.75));
    qDebug() << "UI set";

    setWindowTitle(tr("Content search") + " - " + OTInformation::name);

    DiscordGameSDK::setModule(tr("Content search"));
    DiscordGameSDK::setStatus(tr("Searches missing files"));
    DiscordGameSDK::setStart();
    DiscordGameSDK::setIcon("wcontentsearch");
    DiscordGameSDK::update();

    if (paths.empty())
        paths << "";

    if (paths.at(0) != "")
    {
        paths.removeDuplicates();
        paths.replaceInStrings("\\", "/");
        ui->lwgUserSearch->addItems(paths);
    }

    ui->btnClearLists->setVisible(false);

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->pgbProgress->setVisible(false);

    if (OTInformation::build != OTBuildOptions::Dev)
        ui->actionAddExamples->setVisible(false);

    dbHandler.setupDatabase(database, ":/rec/data/db/contentSearch.db");

    reloadTabNames();

    ui->twgExtras->setTabVisible(3, false);

    if (parent != nullptr)
        ui->actionBackToHome->setEnabled(false);

    qInfo().noquote() << objectName() + " started";
}

wContentSearch::~wContentSearch()
{
    delete ui;
}

void wContentSearch::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->show();
}

void wContentSearch::on_actionClose_triggered()
{
    QApplication::quit();
}

void wContentSearch::on_btnRemove_clicked()
{
    on_actionRemoveSelection_triggered();
}

void wContentSearch::on_btnSearch_clicked()
{
    on_actionSearch_triggered();
}

void wContentSearch::on_actionRemoveSelection_triggered()
{
    if ((ui->lwgUserSearch->selectedItems().size() != 0))
    {
        qDeleteAll(ui->lwgUserSearch->selectedItems());
        qDebug() << "Deleted selected files.";
    }
}

void wContentSearch::reloadTabNames()
{
    ui->twgExtras->setTabText(0, tr("Links (%1)").arg(ui->lwgLinks->count()));
    ui->twgExtras->setTabText(1, tr("Not found (%1)").arg(ui->lwgNotFound->count()));
    ui->twgExtras->setTabText(2, tr("Standard content (%1)", "Please use plural; Optional original text: 'Standard files'").arg(ui->lwgStandardContent->count()));
    ui->twgExtras->setTabText(3, tr("Direct links (%1)").arg(ui->lwgDirectLinks->count()));
}

void wContentSearch::on_actionSearch_triggered()
{
    if (ui->lwgUserSearch->count() == 0)
    {
        ui->statusbar->showMessage(tr("No files available for searching."), 5000);
        return;
    }
    clearView(true);

    QStringList linkIDs;

    ui->pgbProgress->setVisible(true);
    ui->pgbProgress->setValue(0);
    ui->pgbProgress->setMaximum(ui->lwgUserSearch->count());

    for (int i = 0; i < ui->lwgUserSearch->count(); i++)
    {
        ui->pgbProgress->setValue(i + 1);
        qApp->processEvents();

        QString current = ui->lwgUserSearch->item(i)->text();
        if (current == "TEST") { linkIDs << "TEST"; continue; }

        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(dbHandler.doAction(QString("SELECT linkID FROM paths WHERE path = '%1'").arg(current), true));
        QString linkID = model->index(0, 0).data().toString();

        if (linkID == "")
            ui->lwgNotFound->addItem(current);
        else if (linkID == "std")
            ui->lwgStandardContent->addItem(current);
        else
            linkIDs << linkID;
    }

    linkIDs.removeDuplicates();

    QStringList links;
    QStringList directLinks;

    foreach (QString current, linkIDs)
    {
        if (current == "TEST") { links << "TEST"; continue; }

        QSqlQueryModel *model = new QSqlQueryModel();

        // Links
        model->setQuery(dbHandler.doAction(QString("SELECT link FROM links WHERE ID = '%1'").arg(current), true));
        links << model->index(0, 0).data().toString();

        // Direct links
        model->setQuery(dbHandler.doAction(QString("SELECT directLinks FROM links WHERE ID = '%1'").arg(current), true));
        directLinks.append(model->index(0, 0).data().toString().split("\n"));
    }

    links.removeDuplicates();
    ui->lwgLinks->addItems(links);

    directLinks.removeDuplicates();
    directLinks.removeAll("\n");
    ui->lwgDirectLinks->addItems(directLinks);

    qInfo().noquote() << QString("Found %1 of %2 links.").arg(QString::number(ui->lwgLinks->count()), QString::number(ui->lwgUserSearch->count()));

    reloadTabNames();

    ui->btnClearLists->setVisible(true);
}

void wContentSearch::on_btnAddList_clicked()
{
    WADDFILES = new wAddFiles(this);
    connect(WADDFILES, &wAddFiles::submitFiles, this, &wContentSearch::recieveSubmittedFiles);
    WADDFILES->show();
}

void wContentSearch::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this, OTLinks::Wiki::contentSearch);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

void wContentSearch::on_btnOpenInBrowser_clicked()
{
    if (ui->lwgLinks->selectedItems().size() != 0)
    {
        foreach (QListWidgetItem *current, ui->lwgLinks->selectedItems())
            QDesktopServices::openUrl(QUrl(current->text()));

        ui->statusbar->showMessage(tr("Open selected URLs..."), 2000);
    }
}

void wContentSearch::on_btnCopy_clicked()
{
    if (ui->lwgLinks->selectedItems().size() != 0)
    {
        QString copytext;
        foreach (QListWidgetItem *current, ui->lwgLinks->selectedItems())
            copytext += current->text() + "\n";

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copytext);

        ui->statusbar->showMessage(tr("Copied selected URLs."), 2000);
    }
}

void wContentSearch::on_btnReportDeadLink_clicked()
{
    QMessageBox::information(this, tr("Report invalid link"), tr("For reporting an invalid link, please visit the support thread or send an e-mail. Thank you for your assistance!"));
    on_actionSendFeedback_triggered();
}

void wContentSearch::on_btnAddFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select file with paths..."), QStandardPaths::displayName(QStandardPaths::DesktopLocation), QString("%1 (*.txt);; %2 (*.*)").arg(tr("Text files"), tr("All files")));

    if (filename == "")
        return;

    QFile file(filename);

    QStringList pathsFromFile;

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QString path = in.readLine();
            if (path == "")
                continue;
            else
                pathsFromFile << path;
        }
        file.close();

        if (QMessageBox::information(this, tr("Add files"), tr("%1 path(s) will be added to the list.").arg(pathsFromFile.size()), QMessageBox::Cancel | QMessageBox::Yes) == QMessageBox::Yes)
            ui->lwgUserSearch->addItems(pathsFromFile);
        else
            ui->statusbar->showMessage(tr("Process aborted."), 4000);
    }
    else
        msg.fileOpenError(this);
}

void wContentSearch::clearView(bool withoutUserInput)
{
    if (!withoutUserInput)
        ui->lwgUserSearch->clear();

    ui->lwgLinks->clear();
    ui->lwgNotFound->clear();
    ui->lwgStandardContent->clear();
    ui->lwgDirectLinks->clear();

    ui->pteInformation->clear();

    reloadTabNames();
    qApp->processEvents();
}

void wContentSearch::on_btnClearLists_clicked()
{
    clearView();
    ui->btnClearLists->setVisible(false);
    ui->pgbProgress->setVisible(false);
}

void wContentSearch::on_lwgLinks_currentTextChanged(const QString &currentText)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(dbHandler.doAction(QString("SELECT information FROM links WHERE link = '%1'").arg(currentText), true));

    QString information = model->index(0, 0).data().toString();

    if (information.contains("%chromeDL%"))
        information.replace("%chromeDL%", tr("Downloading might not work with Google Chrome.") + "\n");
    else if (information.contains("%directDL%"))
        information.replace("%directDL%", tr("Direct download link - the download will start directly after opening the URL.") + "\n");
    else if (information.contains("%PASSWORD%"))
        information.replace("%PASSWORD%", tr("Archive password:") + " ");

    ui->pteInformation->setPlainText(information);
}

void wContentSearch::on_actionBackToHome_triggered()
{
    close();
    backToHome();
}

void wContentSearch::on_actionAddExamples_triggered()
{
    ui->lwgUserSearch->addItem("Sceneryobjects/Bamp/Wegweiser/O_Wegweiser_Zwischen_geradeaus.sco");
    ui->lwgUserSearch->addItem("Sceneryobjects/Martiesim/VzKat 2017/101-11-G1.sco");
    ui->lwgUserSearch->addItem("Sceneryobjects/BahnFan2014/DB_Stationsschild/model/db_schild.o3d"); // chromeDL
    ui->lwgUserSearch->addItem("Sceneryobjects/Static_Vehicles_DL/texture/DL05_07.tga"); // Password
    ui->lwgUserSearch->addItem("Sceneryobjects/Bamp/St. Peter-Ording/1_Betriebshofhalle.sco"); // not found

    ui->lwgUserSearch->addItem("Sceneryobjects/Rumpelhans/Verkehrszeichen/DreieckU.sco");
    ui->lwgUserSearch->addItem("maps/Berlin-Spandau/Chrono/0200_Grenzoeffnung/tile_2411_11276.map"); // std
    ui->lwgUserSearch->addItem("Sceneryobjects/Steven Objecten/Model/SG_WinkelBlok1.X");
}

void wContentSearch::recieveSubmittedFiles(QStringList files)
{
    ui->lwgUserSearch->addItems(files);
}
