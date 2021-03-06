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
    resize(misc.sizeWindow(0.5, 0.75));
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("Content search"));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    if (paths.empty())
        paths << "";

    if (paths.at(0) != "")
    {
        paths.removeDuplicates();
        paths.replaceInStrings("\\", "/");
        ui->lwgUserSearch->addItems(paths);
    }

    ui->gbxAddList->setVisible(false);
    ui->btnClearLists->setVisible(false);

    ui->statusbar->addPermanentWidget(ui->pgbProgress);
    ui->pgbProgress->setVisible(false);

    if (!database.open())
    {
        qCritical() << "Could not open link database!";
        qDebug() << "Temporary file";
        QMessageBox::critical(this, tr("Content search"), tr("There was an error while open the link database. Please try it again or contect the developer."));
        close();
        return;
    }
    else
    {
        QFile databaseInRessources(dbPath);
        if (!databaseInRessources.open(QFile::ReadOnly))
        {
            qCritical() << "Could not open link database!";
            QMessageBox::critical(this, tr("Content search"), tr("There was an error while open the link database. Please try it again or contect the developer."));
            close();
            return;
        }
        else
        {
            QByteArray data = databaseInRessources.readAll();
            database.write(data);

            databaseInRessources.close();
        }
    }

    dbHandler.dbPath = database.fileName();
    dbHandler.setupDatabase();

    reloadTabNames();

    ui->twgExtras->setTabVisible(3, false);

    qInfo().noquote() << objectName() + " started";


    // Preview:
    {
//        ui->lwgUserSearch->addItem("Sceneryobjects/Bamp/Wegweiser/O_Wegweiser_Zwischen_geradeaus.sco");
//        ui->lwgUserSearch->addItem("Sceneryobjects/Martiesim/VzKat 2017/101-11-G1.sco");
//        ui->lwgUserSearch->addItem("Sceneryobjects/BahnFan2014/DB_Stationsschild/model/db_schild.o3d"); // chromeDL
//        ui->lwgUserSearch->addItem("Sceneryobjects/Static_Vehicles_DL/texture/DL05_07.tga"); // Password
//        ui->lwgUserSearch->addItem("Sceneryobjects/Bamp/St. Peter-Ording/1_Betriebshofhalle.sco"); // not found

//        ui->lwgUserSearch->addItem("Sceneryobjects/Rumpelhans/Verkehrszeichen/DreieckU.sco");
//        ui->lwgUserSearch->addItem("maps/Berlin-Spandau/Chrono/0200_Grenzoeffnung/tile_2411_11276.map"); // std
//        ui->lwgUserSearch->addItem("Sceneryobjects/Steven Objecten/Model/SG_WinkelBlok1.X");
    }
}

wContentSearch::~wContentSearch()
{
    delete ui;
}

/// Redirect
void wContentSearch::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings();
    WSETTINGS->show();
}

/// Redirect
void wContentSearch::on_actionClose_triggered()
{
    QApplication::quit();
}

/// Redirect
void wContentSearch::on_btnRemove_clicked()
{
    on_actionRemoveSelection_triggered();
}

/// Redirect
void wContentSearch::on_btnSearch_clicked()
{
    on_actionSearch_triggered();
}

/// Removes current selection from user's input
void wContentSearch::on_actionRemoveSelection_triggered()
{
    if ((ui->lwgUserSearch->selectedItems().count() != 0) && msg.confirmDeletion(this))
    {
        ui->gbxAddList->setVisible(false);
        qDeleteAll(ui->lwgUserSearch->selectedItems());
        qDebug() << "Deleted selected files.";
    }
}

/// Searchs for user input
void wContentSearch::reloadTabNames()
{
    ui->twgExtras->setTabText(0, tr("Links (%1)").arg(ui->lwgLinks->count()));
    ui->twgExtras->setTabText(1, tr("Not found (%1)").arg(ui->lwgNotFound->count()));
    ui->twgExtras->setTabText(2, tr("Standard content (%1)", "Please use plural; Optional original text: 'Standard files'").arg(ui->lwgStandardContent->count()));
    ui->twgExtras->setTabText(3, tr("Direct links (%1)").arg(ui->lwgDirectLinks->count()));
}

void wContentSearch::on_actionSearch_triggered()
{
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

        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(dbHandler.doAction(QString("SELECT linkID FROM paths WHERE path = '%1'").arg(current)));
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
        if (current == "std")
            continue;

        QSqlQueryModel *model = new QSqlQueryModel();

        // Links
        model->setQuery(dbHandler.doAction(QString("SELECT link FROM links WHERE ID = '%1'").arg(current)));
        links << model->index(0, 0).data().toString();

        // Direct links
        model->setQuery(dbHandler.doAction(QString("SELECT directLinks FROM links WHERE ID = '%1'").arg(current)));
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

/// Adds lists to user input
void wContentSearch::on_btnAddList_clicked()
{
    ui->gbxAddList->setVisible(true);
    ui->pteAddList->setFocus();
}

/// Opens bug report module
void wContentSearch::on_actionSendFeedback_triggered()
{
    wFeedback *WFEEDBACK = new wFeedback(this);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

/// Adds paths from a list to the search list
void wContentSearch::on_btnAddListToList_clicked()
{
    QStringList paths;

    if ((ui->pteAddList->toPlainText() != "") || (ui->pteAddList->toPlainText() != "\n"))
    {
        paths << QString(ui->pteAddList->toPlainText()).split("\n");

        paths.removeDuplicates();
        paths.removeAll("");
        paths.replaceInStrings("\\", "/");
    }

    ui->gbxAddList->setVisible(false);
    ui->pteAddList->clear();
    ui->lwgUserSearch->addItems(paths);
}

/// Opens the selected URL in Browser / file system
void wContentSearch::on_btnOpenInBrowser_clicked()
{
    if (ui->lwgLinks->selectedItems().count() != 0)
    {
        foreach (QListWidgetItem *current, ui->lwgLinks->selectedItems())
            QDesktopServices::openUrl(QUrl(current->text()));

        ui->statusbar->showMessage(tr("Open selected URLs..."), 2000);
    }
}

/// Copies links
void wContentSearch::on_btnCopy_clicked()
{
    if (ui->lwgLinks->selectedItems().count() != 0)
    {
        QString copytext;
        foreach (QListWidgetItem *current, ui->lwgLinks->selectedItems())
            copytext += current->text() + "\n";

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copytext);

        ui->statusbar->showMessage(tr("Copied selected URLs."), 2000);
    }
}

/// Opens feedback
void wContentSearch::on_btnReportDeathLink_clicked()
{
    wFeedback *WFEEDBACK = new wFeedback(this);
    WFEEDBACK->setWindowModality(Qt::ApplicationModal);
    WFEEDBACK->show();
}

/// Adds paths from a file via fileDialog
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

        if (QMessageBox::information(this, tr("Add files"), tr("%1 path(s) will be added to the list.").arg(pathsFromFile.count()), QMessageBox::Cancel | QMessageBox::Yes) == QMessageBox::Yes)
            ui->lwgUserSearch->addItems(pathsFromFile);
        else
            ui->statusbar->showMessage(tr("Process aborted."), 4000);
    }
    else
        msg.fileOpenError(this);
}

/// Clears view
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

/// Clear all lists from user action
void wContentSearch::on_btnClearLists_clicked()
{
    clearView();
    ui->btnClearLists->setVisible(false);
    ui->pgbProgress->setVisible(false);
}

/// Sets information for selected link
void wContentSearch::on_lwgLinks_currentTextChanged(const QString &currentText)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(dbHandler.doAction(QString("SELECT information FROM links WHERE link = '%1'").arg(currentText)));

    QString information = model->index(0, 0).data().toString();

    if (information.contains("%chromeDL%"))
        information.replace("%chromeDL%", tr("Downloading might not work with Google Chrome.") + "\n");
    else if (information.contains("%directDL%"))
        information.replace("%directDL%", tr("Direct download link - the download will start directly after opening the URL.") + "\n");
    else if (information.contains("%PASSWORD%"))
        information.replace("%PASSWORD%", tr("Archive password:") + " ");

    ui->pteInformation->setPlainText(information);
}

