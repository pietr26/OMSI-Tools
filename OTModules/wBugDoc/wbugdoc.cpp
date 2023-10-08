#include "wbugdoc.h"
#include "ui_wbugdoc.h"

wBugDoc::wBugDoc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wBugDoc)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - BugDoc");

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    ui->centralwidget->setEnabled(false);
    ui->lblUnsavedChanges->setVisible(false);

    qInfo().noquote() << objectName() + " started";
}

wBugDoc::~wBugDoc()
{
    delete ui;
}

void wBugDoc::on_actionBack_to_home_triggered()
{
    close();
    emit backToHome();
}


void wBugDoc::on_actionClose_triggered()
{
    QApplication::quit();
}

void wBugDoc::on_actionLoad_triggered()
{
    ui->centralwidget->setEnabled(false);
    QString result = QFileDialog::getExistingDirectory(this, tr("Select / create BugDoc folder..."), projectFolder);

    if (!QDir(result).exists())
    {
        QMessageBox::warning(this, tr("Directory not found"), tr("The selected directory doesn't exists."));
        return;
    }

    projectFolder = result;
    dbHandler.dbPath = projectFolder + "/data/data.db";

    if (!QFile(projectFolder + "/data/data.db").exists())
    {
        QDir().mkdir(projectFolder + "/data");
        dbHandler.setupDatabase(true);
        dbHandler.doAction("CREATE TABLE bugs (ID INTEGER, title TEXT, description TEXT, location TEXT, picturePath TEXT, PRIMARY KEY(ID AUTOINCREMENT))", true);
    }
    else
    {
        dbHandler.setupDatabase();
        loadUI();
    }

    ui->centralwidget->setEnabled(true);
}

void wBugDoc::loadUI()
{
    QSqlQueryModel *bugModel = new QSqlQueryModel();
    dbHandler.openDB();
    bugModel->setQuery(dbHandler.doAction("SELECT ID, title FROM bugs"));
    ui->tvwBugs->setModel(bugModel);
    dbHandler.closeDB();
}

void wBugDoc::loadSingleItem()
{
    int id = ui->tvwBugs->selectionModel()->selectedRows().at(0).data().toInt();

    dbHandler.openDB();
    QSqlQueryModel *singleBugModel = new QSqlQueryModel();
    singleBugModel->setQuery(dbHandler.doAction(QString("SELECT * FROM bugs WHERE ID = %1").arg(id)));

    ui->ledID->setText(singleBugModel->index(0, 0).data().toString());
    ui->ledTitle->setText(singleBugModel->index(0, 1).data().toString());
    ui->pteDescription->setPlainText(singleBugModel->index(0, 2).data().toString());
    ui->ledLocation->setText(singleBugModel->index(0, 3).data().toString());
    ui->ledPicturePath->setText(singleBugModel->index(0, 4).data().toString());
    loadPicture();

    dbHandler.closeDB();
    ui->lblUnsavedChanges->setVisible(false);
}

void wBugDoc::clearSingleBug()
{
    ui->ledID->clear();
    ui->ledTitle->clear();
    ui->pteDescription->clear();
    ui->ledLocation->clear();
    ui->ledPicturePath->clear();
    ui->lblPicture->setPixmap(QPixmap());

    ui->lblUnsavedChanges->setVisible(false);
}

void wBugDoc::loadPicture()
{
    ui->lblPicture->setPixmap(QPixmap(ui->ledPicturePath->text()));
}

void wBugDoc::on_tvwBugs_activated(const QModelIndex &index)
{
    Q_UNUSED(index);
    loadSingleItem();
}

void wBugDoc::on_btnSaveEntry_clicked()
{
    dbHandler.openDB();
    dbHandler.doAction(QString("UPDATE bugs SET title = \"%2\", description = \"%3\", location = \"%4\", picturePath = \"%5\" WHERE ID = %1").arg(ui->ledID->text(), ui->ledTitle->text(), ui->pteDescription->toPlainText(), ui->ledLocation->text(), ui->ledPicturePath->text()));
    dbHandler.closeDB();

    ui->lblUnsavedChanges->setVisible(false);

    loadUI();
    QModelIndex currentIndex = ui->tvwBugs->currentIndex();
    ui->tvwBugs->setCurrentIndex(currentIndex);
}

void wBugDoc::on_ledID_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lblUnsavedChanges->setVisible(true);
}

void wBugDoc::on_ledTitle_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lblUnsavedChanges->setVisible(true);
}

void wBugDoc::on_pteDescription_textChanged()
{
    ui->lblUnsavedChanges->setVisible(true);
}

void wBugDoc::on_ledLocation_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lblUnsavedChanges->setVisible(true);
}

void wBugDoc::on_ledPicturePath_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->lblUnsavedChanges->setVisible(true);
    loadPicture();
}


void wBugDoc::on_btnDelete_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Confirm deletion"), tr("Really delete the selected bug entry?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if (reply != QMessageBox::Yes) return;

    int currentRow = ui->tvwBugs->currentIndex().row();

    dbHandler.openDB();
    QSqlQueryModel *singleBugModel = new QSqlQueryModel();
    singleBugModel->setQuery(dbHandler.doAction(QString("DELETE FROM bugs WHERE ID = %1").arg(ui->tvwBugs->selectionModel()->selectedRows().at(0).data().toInt())));
    dbHandler.closeDB();

    loadUI();
    clearSingleBug();

    try
    {
        ui->tvwBugs->selectRow(currentRow);
    }
    catch(...) { }
}


void wBugDoc::on_btnAdd_clicked()
{
    dbHandler.openDB();
    dbHandler.doAction("INSERT INTO bugs (title) VALUES (\"\")");
    dbHandler.closeDB();

    loadUI();
    ui->tvwBugs->selectRow(ui->tvwBugs->model()->rowCount() - 1);
    loadSingleItem();
}

