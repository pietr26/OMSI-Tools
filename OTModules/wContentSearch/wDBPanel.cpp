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

    // Load settings
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
//    standardFilter << "*.water";
//    standardFilter << "*.rdy";
//    standardFilter << "*.opl";
//    standardFilter << "*.owt";
//    standardFilter << "*.osn";
//    standardFilter << "*.dsc";

    // Setup model
    strListModelDuplicates = new QStringListModel();
    strListModelItems = new QStringListModel();

    reloadSelectGroupBoxes();
    ui->btnUnlockSource->setVisible(false);

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

    qApp->processEvents();
    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 1);

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
        if (directLinks.isEmpty())
            dbHandler.doAction(QString("UPDATE links SET information = '%1' WHERE ID = %2").arg(information, qryLinkModel->index(0, 0).data().toString()), true);

        if (information.isEmpty())
            dbHandler.doAction(QString("UPDATE links SET directLinks = '%1' WHERE ID = %2").arg(directLinks, qryLinkModel->index(0, 0).data().toString()), true);
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

    ui->gbxDirectLinks->setEnabled(false);
    ui->gbxInformation->setEnabled(false);
    ui->gbxSource->setEnabled(false);

    ui->btnStart->setEnabled(false);
    ui->wgtDuplicates->setEnabled(true);
    ui->btnUnlockSource->setVisible(true);

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

    while (dirIterator.hasNext())
    {
        ui->lblProgress->setText("0 / " + QString::number(files.size()));
        qApp->processEvents();

        files << QFileInfo(dirIterator.next()).absoluteFilePath().remove(0, cutCount);
    }
    qInfo().noquote() << "File counter finished.";

    files.sort();

    strListModelItems->setStringList(files);
    ui->lvwInputItems->setModel(strListModelItems);

    dbHandler.createBackup();

    ui->pgbProgress->setMaximum(files.size());
    ui->pgbProgress->setValue(0);
    unsigned int i = 0;

    qInfo().noquote() << "Starting database comparision / appending...";

    QElapsedTimer timer;
    timer.start();

    dbHandler.doAction("CREATE INDEX indexPaths ON paths (path)", true);

    QString insertAction = "INSERT INTO paths (path, linkID) VALUES";

    foreach (QString current, files)
    {
        i++;
        qApp->processEvents();

        ui->lblProgress->setText(QString::number(i) + " / " + QString::number(files.size()));
        ui->pgbProgress->setValue(i);

        QSqlQueryModel *qryModel = new QSqlQueryModel;

        qryModel->setQuery(dbHandler.doAction(QString("SELECT * FROM paths WHERE path = '%1'").arg(current), true));
        if (qryModel->rowCount() == 0)
        {
            if (!ui->rbtnStandardContent->isChecked())
                currentLinkID = checkLinkID();
            else
                currentLinkID = "'std'";

            insertAction.append(QString(" ('%1', %2),").arg(current, currentLinkID));
        }
        else
        {
            models << qryModel;
            paths << current;
        }
    }

    dbHandler.doAction("DROP INDEX indexPaths", true);

    insertAction.chop(1);
    dbHandler.doAction(insertAction, true);

    strListModelDuplicates->setStringList(paths);
    ui->lvwDuplicates->setModel(strListModelDuplicates);
    setEnabled(true);
    reloadSelectGroupBoxes();

    ui->statusbar->showMessage(QString("Finished - Needed %1 sec for %2 files").arg(QString::number(timer.elapsed() / 1000), QString::number(files.size())));

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
    ui->gbxDirectLinks->setEnabled(true);
    ui->pteDirectLinks->clear();

    ui->gbxInformation->setEnabled(true);
    ui->ledInformation->clear();

    ui->gbxSource->setEnabled(true);
    ui->ledLink->clear();

    ui->btnStart->setEnabled(true);

    ui->wgtDuplicates->setEnabled(false);

    ui->btnUnlockSource->setVisible(false);
}


void wDBPanel::on_actionBackToHome_triggered()
{
    close();
    backToHome();
}
