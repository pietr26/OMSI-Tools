#include "wDBPanel.h"
#include "ui_wDBPanel.h"

wDBPanel::wDBPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wDBPanel)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - database panel");
    ui->btnStart->setEnabled(false);
    ui->pgbProgress->setVisible(false);

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    // Set filters
    standardFilter << "*.sco";
    standardFilter << "*.sli";
    standardFilter << "*.bus";
    standardFilter << "*.zug";
    standardFilter << "*.ovh";
    standardFilter << "*.x";
    standardFilter << "*.o3d";
    standardFilter << "*.png";
    standardFilter << "*.jpg";
    standardFilter << "*.jpeg";
    standardFilter << "*.dds";
    standardFilter << "*.bmp";
    standardFilter << "*.tga";
    standardFilter << "*.txt";
    standardFilter << "*.cfg";
    standardFilter << "*.osc";
    standardFilter << "*.dll";
    standardFilter << "*.wav";
    standardFilter << "*.oft";
    standardFilter << "*.hum";
    standardFilter << "*.map";
    standardFilter << "*.terrain";

    // Since 2023-01-16:
    standardFilter << "*.org";
//    standardFilter << "*.water";
//    standardFilter << "*.rdy";
//    standardFilter << "*.opl";
//    standardFilter << "*.owt";
//    standardFilter << "*.osn";
//    standardFilter << "*.dsc";

    ui->statusbar->addPermanentWidget(ui->pgbProgress);

    // Setup model
    strListModelDuplicates = new QStringListModel();
    strListModelItems = new QStringListModel();

    reloadSelectGroupBoxes();
    ui->btnUnlockSource->setVisible(false);
    ui->gbxSelectAll->setEnabled(false);
    ui->gbxSelectSingle->setEnabled(false);

    ui->tvwDuplicates->verticalHeader()->hide();

    // Setup database
    dbHandler.dbPath = dbPath;
    if (!QFile(QDir(dbPath).absolutePath()).exists())
    {
        dbHandler.setupDatabase(true);
        dbHandler.doAction("CREATE TABLE paths (ID INTEGER, path varchar(5000), linkID varchar(5000), PRIMARY KEY(ID AUTOINCREMENT))", true);
        dbHandler.doAction("CREATE TABLE links (ID INTEGER, link varchar(5000), directLinks varchar(5000), information varchar(5000),PRIMARY KEY(ID AUTOINCREMENT))", true);
    }
    else
        dbHandler.setupDatabase();

    ui->ledDirectory->setText(set.read(objectName(), "tempPath").toString());

    qInfo().noquote() << objectName() + " started";
}

wDBPanel::~wDBPanel()
{
    delete strListModelDuplicates;
    delete strListModelItems;

    delete ui;
}

/// Slot for Link entering
void wDBPanel::on_rbtnLink_clicked()
{
    ui->ledLink->setEnabled(true);
}

/// Slot for Link entering
void wDBPanel::on_rbtnStandardContent_clicked()
{
    ui->ledLink->setEnabled(false);
}

/// Starts analysing
void wDBPanel::reloadSelectGroupBoxes()
{
    if (strListModelDuplicates->rowCount() == 0)
    {
        ui->gbxSelectAll->setEnabled(false);
        ui->gbxSelectSingle->setEnabled(false);
    }
    else
    {
        ui->gbxSelectAll->setEnabled(true);
        ui->gbxSelectSingle->setEnabled(true);
    }
}

QString wDBPanel::checkLinkID()
{
    QSqlQueryModel *qryLinkModel = new QSqlQueryModel;

    QString link;
    if (ui->rbtnLink->isChecked())
        link = ui->ledLink->text();
    else {
        delete qryLinkModel;
        return "NULL";
    }

    QString directLinks = ui->pteDirectLinks->toPlainText();

    QString information = ui->ledInformation->text();

    qryLinkModel->setQuery(dbHandler.doAction(QString("SELECT * FROM links WHERE link = '%1'").arg(link), true));

    bool isEmpty = false;

    if (qryLinkModel->rowCount() == 0)
    {
        QString dbAction = QString("INSERT INTO links (link, directLinks, information) VALUES ('%1'").arg(link);

        if (directLinks.isEmpty())
            dbAction.append(QString(", NULL"));
        else
            dbAction.append(QString(", '%1'").arg(directLinks));

        if (information.isEmpty())
            dbAction.append(QString(", NULL"));
        else
            dbAction.append(QString(", '%1'").arg(information));

        dbAction.append(")");

        isEmpty = true;
        dbHandler.doAction(dbAction, true);
        qDebug() << dbAction;
    }

    qryLinkModel->setQuery(dbHandler.doAction(QString("SELECT ID FROM links WHERE link = '%1'").arg(link), true));

    if (!isEmpty)
    {
        if (directLinks.isEmpty() || information.isEmpty())
            dbHandler.doAction(QString("UPDATE links SET information = '%1' WHERE ID = %2").arg(information, qryLinkModel->index(0, 0).data().toString()), true);
    }

    return qryLinkModel->index(0, 0).data().toString();
}

void wDBPanel::on_btnStart_clicked()
{
    if ((!QDir(ui->ledDirectory->text()).exists()) || (ui->ledDirectory->text() == ""))
    {
        QMessageBox::warning(this, "Invalid or no path", "The selected path doesn't exist!");
        ui->statusbar->showMessage("The selected path doesn't exist!", 4000);
        return;
    }

    if (ui->rbtnLink->isChecked() && ui->ledLink->text().isEmpty())
    {
        QMessageBox::warning(this, "No link entered", "Link field is empty!");
        ui->statusbar->showMessage("Link field is empty!", 4000);
        return;
    }

    ui->gbxFileProperties->setEnabled(false);

    ui->btnStart->setEnabled(false);
    ui->btnUnlockSource->setVisible(true);
    ui->gbxSelectAll->setEnabled(true);
    ui->gbxSelectSingle->setEnabled(true);

    models.clear();
    paths.clear();
    currentLinkID.clear();

    dbHandler.setupDatabase();

    setEnabled(false);
    ui->pgbProgress->setVisible(true);
    ui->pgbProgress->setMinimum(0);
    ui->pgbProgress->setMaximum(0);

    QDirIterator dirIterator(ui->ledDirectory->text(), standardFilter, QDir::Files, QDirIterator::Subdirectories);

    QStringList files;

    qInfo().noquote() << "Starting file counter...";
    ui->statusbar->showMessage("File counter...");

    while (dirIterator.hasNext())
    {
        QString next = dirIterator.next();
        if (next.contains(".map.LM.bmp")) continue;
        ui->pgbProgress->setMaximum(files.size());
        qApp->processEvents();
        files << QFileInfo(next).absoluteFilePath().remove(0, cutCount);
    }

    qInfo().noquote() << "File counter finished.";
    files.sort();

    strListModelItems->setStringList(files);
    ui->lvwInputItems->setModel(strListModelItems);

    ui->pgbProgress->setMaximum(files.size());
    ui->pgbProgress->setValue(0);
    unsigned int i = 0;

    ui->statusbar->showMessage("Compare data with database...");

    qInfo().noquote() << "Starting database comparision / appending...";

    QElapsedTimer timer;
    timer.start();

    dbHandler.doAction("CREATE INDEX indexPaths ON paths (path)", true);

    QStringList insertActions;
    insertActions.append("INSERT INTO paths (path, linkID) VALUES");
    int iList = 0;

    foreach (QString current, files)
    {
        i++;
        qApp->processEvents();

        ui->pgbProgress->setValue(i);

        QSqlQueryModel *qryModel = new QSqlQueryModel;

        qryModel->setQuery(dbHandler.doAction(QString("SELECT * FROM paths WHERE path = '%1'").arg(current), true));
        if (qryModel->rowCount() == 0)
        {
            if (!ui->rbtnStandardContent->isChecked())
                currentLinkID = checkLinkID();
            else
                currentLinkID = "'std'";

            insertActions[iList] += (QString(" ('%1', %2),").arg(current, currentLinkID));
        }
        else
        {
            models << qryModel;
            paths << current;
        }

        if (insertActions.at(iList).length() > 50000)
        {
            iList++;
            insertActions.append("INSERT INTO paths (path, linkID) VALUES");
        }
    }

    dbHandler.doAction("DROP INDEX indexPaths", true);

    ui->statusbar->showMessage("Execute insert actions...");
    qApp->processEvents();

    foreach (QString current, insertActions)
    {
        current.chop(1);
        dbHandler.doAction(current, true);
    }

    strListModelDuplicates->setStringList(paths);
    ui->lvwDuplicates->setModel(strListModelDuplicates);
    setEnabled(true);
    reloadSelectGroupBoxes();

    ui->statusbar->showMessage(QString("Done - Files: %1, Time: %2s, insertActionCount: %3").arg(QString::number(files.size()), QString::number(timer.elapsed() / 1000), QString::number(iList + 1)));

    qInfo() << "database comparision / appending finished.";
}

/// Closes the window
void wDBPanel::on_actionClose_triggered()
{
    close();
}

/// Slot for directory changes
void wDBPanel::on_ledDirectory_textChanged(const QString &arg1)
{
    set.write(objectName(), "tempPath", arg1);
    cutCount = arg1.size() + 1;

    if (arg1 == "")
        ui->btnStart->setEnabled(false);
    else
        ui->btnStart->setEnabled(true);
}

/// Changes the current duplicate item
void wDBPanel::on_lvwDuplicates_clicked(const QModelIndex &index)
{
    ui->tvwDuplicates->setModel(models.at(index.row()));
    ui->tvwDuplicates->resizeColumnsToContents();
    ui->tvwDuplicates->resizeRowsToContents();
}

/// Opens a folder to scan
void wDBPanel::on_btnOpenFolder_clicked()
{
    ui->ledDirectory->setText(QFileDialog::getExistingDirectory(this, "", set.read(objectName(), "tempPath").toString()));
}

/// Removes current from duplicate list
void wDBPanel::removeCurrentFromList(bool all)
{
    if (all)
    {
        models.clear();
        paths.clear();

        strListModelDuplicates->setStringList(paths);
        ui->lvwDuplicates->setModel(strListModelDuplicates);
    }
    else
    {
        int at = ui->lvwDuplicates->currentIndex().row();
        models.removeAt(at);
        paths.removeAt(at);
    }

    strListModelDuplicates->setStringList(paths);
    ui->lvwDuplicates->setModel(strListModelDuplicates);
    reloadSelectGroupBoxes();

    // Put in an empty model to the single view
    QSqlQueryModel *model = new QSqlQueryModel();
    model->clear();
    ui->tvwDuplicates->setModel(model);
}

/// Selects the new one from a duplicate
void wDBPanel::on_btnSelectNew_clicked()
{
    if (ui->lvwDuplicates->currentIndex().row() == -1)
        return;

    selectNew();
}

/// Selects the old one from a duplicate
void wDBPanel::on_btnSelectOld_clicked()
{
    removeCurrentFromList();
}

/// Select all new entries
void wDBPanel::on_btnSelectAllNew_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Confirm action", "Do you really want to select <b>all NEW</b> entries?", QMessageBox::YesAll | QMessageBox::Cancel);

    if (reply != QMessageBox::YesAll)
        return;

    ui->pgbProgress->setValue(0);
    ui->pgbProgress->setMaximum(0);

    while (strListModelDuplicates->rowCount() >= 1)
    {
        ui->statusbar->showMessage(QString("%1 elements remaining...").arg(strListModelDuplicates->rowCount()), 1000);
        qApp->processEvents();
        ui->lvwDuplicates->setCurrentIndex(strListModelDuplicates->index(0));

        selectNew();
    }

    ui->pgbProgress->setValue(1);
    ui->pgbProgress->setMaximum(1);
}

/// Select all old entries
void wDBPanel::on_btnSelectAllOld_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Confirm action", "Do you really want to select <b>all OLD</b> entries?", QMessageBox::YesAll | QMessageBox::Cancel);

    if (reply != QMessageBox::YesAll)
        return;

    ui->pgbProgress->setValue(0);
    ui->pgbProgress->setMaximum(0);

    removeCurrentFromList(true);

    ui->pgbProgress->setValue(0);
    ui->pgbProgress->setMaximum(1);

    ui->statusbar->showMessage("Finished.");
}

/// Merge function for select new buttons
void wDBPanel::selectNew()
{
    dbHandler.doAction(QString("DELETE FROM paths WHERE path='%1'").arg(ui->lvwDuplicates->currentIndex().data().toString()), true);

    currentLinkID = checkLinkID();

    dbHandler.doAction(QString("INSERT INTO paths (path, linkID) VALUES ('%1', %2)").arg(ui->lvwDuplicates->currentIndex().data().toString(), currentLinkID), true);

    removeCurrentFromList();
}

/// Unlocks the source group box
void wDBPanel::on_btnUnlockSource_clicked()
{
    ui->gbxFileProperties->setEnabled(true);
    ui->pteDirectLinks->clear();
    ui->ledInformation->clear();
    ui->ledLink->clear();

    ui->btnStart->setEnabled(true);

    ui->btnUnlockSource->setVisible(false);
    ui->gbxSelectAll->setEnabled(false);
    ui->gbxSelectSingle->setEnabled(false);
}


void wDBPanel::on_actionBackToHome_triggered()
{
    close();
    backToHome();
}

void wDBPanel::on_actionStandard_triggered()
{
    adjustSize();
}

void wDBPanel::on_actionWidth1Height05_triggered()
{
    resize(misc.sizeWindow(1, 0.5));
}

void wDBPanel::on_actionCreateBackup_triggered()
{
    dbHandler.createBackup();
    ui->statusbar->showMessage("Backup created.", 5000);
}
