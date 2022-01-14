#include "wcontentsearch.h"
#include "ui_wcontentsearch.h"

wContentSearch::wContentSearch(QWidget *parent, QStringList paths) :
    QMainWindow(parent),
    ui(new Ui::wContentSearch)
{
    qInfo().noquote() << "Starting " + moduleName + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
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

    ui->gbxAddFile->setVisible(false);
    ui->gbxAddList->setVisible(false);
    ui->btnClearLists->setVisible(false);

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

    // Preview:
    {
//        ui->lwgUserSearch->addItem("Sceneryobjects/Bamp/Wegweiser/O_Wegweiser_Zwischen_geradeaus.sco");
//        ui->lwgUserSearch->addItem("Sceneryobjects/Martiesim/VzKat 2017/101-11-G1.sco");
//        ui->lwgUserSearch->addItem("Sceneryobjects/Martiesim/VzKat 2017/267-G3.sco");
//        ui->lwgUserSearch->addItem("Sceneryobjects/Martiesim/VzKat 2017/Mast-Verbinder-120-DB-H.sco");
//        ui->lwgUserSearch->addItem("Sceneryobjects/Bamp/St. Peter-Ording/1_Betriebshofhalle.sco");

//        ui->lwgLinks->addItem("https://reboot.omsi-webdisk.de/file/5005-radwegweiser-cycling-signpost/");
//        ui->lwgLinks->addItem("https://reboot.omsi-webdisk.de/file/1619-virtueller-schilderwald-vzkat-2017/");

//        ui->lwgNotFound->addItem("Sceneryobjects/Bamp/St. Peter-Ording/1_Betriebshofhalle.sco");
//        ui->lwgNotFound->item(0)->setForeground(Qt::red);
    }

    dbHandler.dbPath = database.fileName();
    dbHandler.setupDatabase();

    qInfo().noquote() << moduleName + " started successfully.";

//#ifndef QT_DEBUG
//    msg.moduleDeactivated(this);
//    QTimer::singleShot(1, this, SLOT(close()));
//#endif
}

wContentSearch::~wContentSearch()
{
    qInfo().noquote() << moduleName + " is closing...";
    delete ui;
}

/// \brief Redirect
void wContentSearch::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings();
    WSETTINGS->show();
}

/// \brief Redirect
void wContentSearch::on_actionClose_triggered()
{
    QApplication::quit();
}

/// \brief Redirect
void wContentSearch::on_btnRemove_clicked()
{
    on_actionRemoveSelection_triggered();
}

/// \brief Redirect
void wContentSearch::on_btnAdd_clicked()
{
    on_actionAddFile_triggered();
}

/// \brief Redirect
void wContentSearch::on_btnSearch_clicked()
{
    on_actionSearch_triggered();
}

/// \brief Removes current selection from user's input
void wContentSearch::on_actionRemoveSelection_triggered()
{
    if (msg.confirmDeletion(this))
    {
        ui->gbxAddFile->setVisible(false);
        qDeleteAll(ui->lwgUserSearch->selectedItems());
        qDebug() << "Deleted selected files.";
    }
}

/// \brief Adds content from led to user's input
void wContentSearch::on_btnAddToList_clicked()
{
    if (ui->gbxAddFile->isVisible())
    {
        QString content = ui->ledPath->text();
        if (content != "")
        {
            content.replace("\\", "/");
            ui->lwgUserSearch->addItem(content);
            ui->gbxAddFile->setVisible(false);
            ui->ledPath->clear();
        }
        else
            ui->statusbar->showMessage(tr("Input is empty."), 4000);
    }
}

/// \brief Searchs for user input
void wContentSearch::on_actionSearch_triggered()
{
    ui->lwgLinks->clear();
    ui->lwgNotFound->clear();

    QStringList linkIDs;
    unsigned int foundLinks = 0;

    for (int i = 0; i < ui->lwgUserSearch->count(); i++)
    {

        QString current = ui->lwgUserSearch->item(i)->text();

        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(dbHandler.doAction(QString("SELECT linkID FROM paths WHERE path = '%1'").arg(current)));
        QString linkID = model->index(0, 0).data().toString();

        if (linkID == "")
        {
            ui->lwgNotFound->addItem(current);
            ui->lwgNotFound->item(ui->lwgNotFound->count() - 1)->setForeground(Qt::red);
        }
        else
        {
            foundLinks++;
            linkIDs << linkID;
        }
    }

    linkIDs.removeDuplicates();

    QStringList links;

    foreach (QString current, linkIDs)
    {
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(dbHandler.doAction(QString("SELECT link FROM links WHERE ID = '%1'").arg(current)));
        links << model->index(0, 0).data().toString();
    }

    links.removeDuplicates();
    ui->lwgLinks->addItems(links);

    qInfo().noquote() << QString("Found %1 of %2 links.").arg(QString::number(foundLinks), QString::number(ui->lwgUserSearch->count()));

    ui->btnClearLists->setVisible(true);
}

/// \brief Shows add file group box
void wContentSearch::on_actionAddFile_triggered()
{
    ui->gbxAddFile->setVisible(true);
    ui->ledPath->setFocus();
}

/// \brief Adds lists to user input
void wContentSearch::on_btnAddList_clicked()
{
    ui->gbxAddList->setVisible(true);
    ui->pteAddList->setFocus();
}

/// \brief Opens bug report module
void wContentSearch::on_actionSendFeedback_triggered()
{
    OTMiscellaneous::sendFeedback();
}

/// \brief Adds paths from a list to the search list
void wContentSearch::on_btnAddListToList_clicked()
{
    QStringList paths;

    ui->gbxAddList->setVisible(false);

    paths << QString(ui->pteAddList->toPlainText()).split("\n");

    paths.removeDuplicates();
    paths.removeAll("");
    paths.replaceInStrings("\\", "/");

    ui->pteAddList->clear();
    ui->lwgUserSearch->addItems(paths);
}

/// \brief Opens the selected URL in Browser / file system
void wContentSearch::on_btnOpenInBrowser_clicked()
{
    if (ui->lwgLinks->selectedItems().count() != 0)
    {
        foreach (QListWidgetItem *current, ui->lwgLinks->selectedItems())
            QDesktopServices::openUrl(QUrl(current->text()));

        ui->statusbar->showMessage(tr("Open URL(s)..."), 2000);
    }
}

/// \brief Copies links
void wContentSearch::on_btnCopy_clicked()
{
    if (ui->lwgLinks->selectedItems().count() != 0)
    {
        QString copytext;
        foreach (QListWidgetItem *current, ui->lwgLinks->selectedItems())
            copytext += current->text() + "\n";

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copytext);

        ui->statusbar->showMessage(tr("Copy URL(s)..."), 2000);
    }
}

/// \brief Opens feedback
void wContentSearch::on_btnReportDeathLink_clicked()
{
    OTMiscellaneous::sendFeedback();
}

/// \brief Adds paths from a file via fileDialog
void wContentSearch::on_btnAddFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select file with paths...", "Note #1"), QStandardPaths::displayName(QStandardPaths::DesktopLocation), QString("%1 (*.txt);; %2 (*.*)").arg(tr("Text files"), tr("All files")));

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

        if (QMessageBox::information(this, tr("Add files", "Note #1"), tr("%1 path(s) will be added to the list.").arg(pathsFromFile.count()), QMessageBox::Cancel | QMessageBox::Yes) == QMessageBox::Yes)
            ui->lwgUserSearch->addItems(pathsFromFile);
        else
            ui->statusbar->showMessage(tr("Process aborted."), 4000);
    }
    else
        msg.errorOpeningFile(this);
}


void wContentSearch::on_btnClearLists_clicked()
{
    ui->lwgLinks->clear();
    ui->lwgNotFound->clear();
    ui->lwgUserSearch->clear();

    ui->btnClearLists->setVisible(false);
}

