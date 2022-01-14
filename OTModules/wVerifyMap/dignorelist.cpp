#include "dignorelist.h"
#include "ui_dignorelist.h"

dIgnoreList::dIgnoreList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dIgnoreList)
{
    qInfo().noquote() << "Starting " + moduleName + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTName + " - " + tr("ignorelist"));

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    unsaved = false;

    // load ignoreList
    QFile file("ignorelist.cfg");
    if (file.exists() && file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QStringList list;
        while (!in.atEnd())
           list << in.readLine();

        list.removeDuplicates();
        list.sort(Qt::CaseInsensitive);
        ui->lwgIgnoreList->addItems(list);
        file.close();
    }
    qInfo().noquote() << moduleName + " started successfully.";;
}

dIgnoreList::~dIgnoreList()
{
    delete ui;
}

void dIgnoreList::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if (unsaved)
        if (msg.unsavedContentYesNo(this))
            save();

    qInfo().noquote() << moduleName + " is closing...";
}

/// \brief Saves the ignoreList
void dIgnoreList::save()
{
    qInfo() << "Save ignorelist...";
    QFile file("ignorelist.cfg");
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);

        if (ui->lwgIgnoreList->count() != 0)
        {
            for (int i = 0; i < ui->lwgIgnoreList->count(); i++)
                out << ui->lwgIgnoreList->item(i)->text() << "\n";
        }

        file.flush();
        file.close();
        unsaved = false;
        qInfo() << "Ignorelist successfully saved!";
    }
    else
    {
        QMessageBox::critical(this, tr("Could not save ignorelist", "Note #1"), tr("There was an error while saving the ignorelist."));
        qCritical() << "Could not save ignorelist!";
        qDebug().noquote() << "To file: '" + QFileInfo(file).absoluteFilePath() + "'";
    }
}

/// \brief Closes ignoreList dialog
void dIgnoreList::on_btnCancel_clicked()
{
    close();
}

/// \brief Adds files to lwgIgnoreList
void dIgnoreList::on_btnAddFiles_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select files..."), set.read("main", "mainDir").toString());
    QStringList newFiles;
    qInfo().noquote() << "Adding " + QString::number(files.count()) + " Files to ignorelist...";

    foreach (QString current, files)
        newFiles << current.remove(0, set.read("main", "mainDir").toString().count() + 1);

    QStringList list;

    try
    {
        // Get items from listview
        for (int i = 0; i < ui->lwgIgnoreList->count(); i++)
            list << ui->lwgIgnoreList->item(i)->text();

        // Append new files to the same StringList
        list.append(newFiles);
    }
    catch (...)
    {
        QMessageBox::critical(this, tr("Could not append ignorelist", "Note #1"), tr("There was an error while append the files to the ignorelist."));
        qCritical() << "Could not append ignorelist!";
        qDebug().noquote().noquote() << "Try to add files: " << newFiles;
        return;
    }

    list.removeDuplicates();
    list.sort(Qt::CaseInsensitive);

    // Deletes all items from listView
    ui->lwgIgnoreList->selectAll();
    qDeleteAll (ui->lwgIgnoreList->selectedItems());

    // Add new list (old and new ones)
    ui->lwgIgnoreList->addItems(list);
    ui->lwgIgnoreList->setCurrentRow(ui->lwgIgnoreList->count() - 1);
    unsaved = true;
    qInfo() << "Successfully added files!";
}

/// \brief Removes selected items
void dIgnoreList::on_btnRemove_clicked()
{
    if (msg.confirmDeletion(this))
    {
        qDeleteAll(ui->lwgIgnoreList->selectedItems());
        ui->lwgIgnoreList->setCurrentRow(ui->lwgIgnoreList->count() - 1);
        unsaved = true;
        qDebug() << "Deleted selected files.";
    }
}

/// \brief Saves and closes the ignoreList dialog
void dIgnoreList::on_btnSave_clicked()
{
    save();
    close();
}

/// \brief Removes all items from the ignoreList
void dIgnoreList::on_btnRemoveAll_clicked()
{
    if (msg.confirmDeletion(this))
    {
        ui->lwgIgnoreList->selectAll();
        qDeleteAll(ui->lwgIgnoreList->selectedItems());
    }
    unsaved = true;
    qDebug() << "Deleted all files.";
}
