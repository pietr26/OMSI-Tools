#include "wdbcopyrights.h"
#include "ui_wdbcopyrights.h"

wDBCopyrights::wDBCopyrights(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wDBCopyrights)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - argument panel");

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    // Setup database
    dbHandler.dbPath = dbPath;
    if (!QFile(QDir(dbPath).absolutePath()).exists())
    {
        dbHandler.setupDatabase(true);
        dbHandler.doAction("CREATE TABLE arguments (ID INTEGER, argument TEXT, PRIMARY KEY(ID AUTOINCREMENT))", true);
        dbHandler.doAction("CREATE TABLE paths (ID INTEGER, path TEXT, argumentIDs TEXT, pathRedirect INTEGER, PRIMARY KEY(ID AUTOINCREMENT))", true);
    }
    else
        dbHandler.setupDatabase();

    updateView();

    qInfo().noquote() << objectName() + " started";
}

wDBCopyrights::~wDBCopyrights()
{
    delete ui;
}

void wDBCopyrights::on_tvwPathPreferences_activated(const QModelIndex &index)
{
    Q_UNUSED(index);

    int ID = ui->tvwPathPreferences->selectionModel()->selectedRows().at(0).data().toInt();
    QString path = ui->tvwPathPreferences->selectionModel()->selectedRows(1).at(0).data().toString();
    QString argumentIDs = ui->tvwPathPreferences->selectionModel()->selectedRows(2).at(0).data().toString();
    int redirect = ui->tvwPathPreferences->selectionModel()->selectedRows(3).at(0).data().toInt();

    wAddPath *addPath = new wAddPath(false, this, ID, path, argumentIDs, redirect);
    addPath->show();
    connect(addPath, &wAddPath::dialogFinished, this, &wDBCopyrights::addPathFinished);
}

void wDBCopyrights::on_tvwCopyrightTerms_activated(const QModelIndex &index)
{
    Q_UNUSED(index);

    int ID = ui->tvwCopyrightTerms->selectionModel()->selectedRows().at(0).data().toInt();
    QString argument = ui->tvwCopyrightTerms->selectionModel()->selectedRows(1).at(0).data().toString();

    wAddTerm *addTerm = new wAddTerm(false, this, ID, argument);
    addTerm->show();
    connect(addTerm, &wAddTerm::dialogFinished, this, &wDBCopyrights::addTermFinished);
}

void wDBCopyrights::on_btnPathPreferencesAdd_clicked()
{
    wAddPath *addPath = new wAddPath(true, this);
    addPath->show();
    connect(addPath, &wAddPath::dialogFinished, this, &wDBCopyrights::addPathFinished);
}


void wDBCopyrights::on_btnPathPreferencesRemove_clicked()
{
    QItemSelectionModel *model = ui->tvwPathPreferences->selectionModel();

    if (model->hasSelection())
    {
        QSqlQueryModel *qryModel = new QSqlQueryModel();
        dbHandler.openDB();
        qryModel->setQuery(dbHandler.doAction(QString("SELECT * FROM paths WHERE pathRedirect = %1").arg(model->selectedRows().at(0).data().toInt())));

        if (qryModel->index(0, 0).data().isNull())
            dbHandler.doAction(QString("DELETE FROM paths WHERE ID = %1").arg(model->selectedRows().at(0).data().toInt()));
        else
            ui->statusbar->showMessage("Path is still a connection target from a redirect!", 5000);

        dbHandler.closeDB();
    }

    updateView();
}


void wDBCopyrights::on_btnCopyrightTermsAdd_clicked()
{
    wAddTerm *addTerm = new wAddTerm(true, this);
    addTerm->show();
    connect(addTerm, &wAddTerm::dialogFinished, this, &wDBCopyrights::addTermFinished);
}

void wDBCopyrights::on_btnCopyrightTermsRemove_clicked()
{
    QItemSelectionModel *model = ui->tvwCopyrightTerms->selectionModel();

    if (model->hasSelection())
    {
        QSqlQueryModel *qryModel = new QSqlQueryModel();
        dbHandler.openDB();
        qryModel->setQuery(dbHandler.doAction(QString("SELECT * FROM paths WHERE argumentIDs LIKE '%|%1|%'").arg(model->selectedRows().at(0).data().toInt())));

        if (qryModel->index(0, 0).data().isNull())
            dbHandler.doAction(QString("DELETE FROM arguments WHERE ID = %1").arg(model->selectedRows().at(0).data().toInt()));
        else
            ui->statusbar->showMessage("Term is still connected with at least one path!", 5000);

        dbHandler.closeDB();
    }

    updateView();
}

void wDBCopyrights::addPathFinished(int ID, QString path, QString argumentIDs, int redirect)
{
    dbHandler.setupDatabase();

    if (ID == -1)
        dbHandler.doAction(QString("INSERT INTO paths (path, argumentIDs, pathRedirect) VALUES ('%1', '%2', %3)").arg(path).arg(argumentIDs).arg(redirect), true);
    else
        dbHandler.doAction(QString("UPDATE paths SET path = '%1', argumentIDs = '%2', pathRedirect = %3 WHERE ID = %4").arg(path).arg(argumentIDs).arg(redirect).arg(ID), true);

    updateView();
}

void wDBCopyrights::addTermFinished(int ID, QString argument)
{
    dbHandler.setupDatabase();

    if (ID == -1)
        dbHandler.doAction(QString("INSERT INTO arguments (argument) VALUES ('%1')").arg(argument), true);
    else
        dbHandler.doAction(QString("UPDATE arguments SET argument = '%1' WHERE ID = %2").arg(argument).arg(QString::number(ID)), true);

    updateView();
}

void wDBCopyrights::updateView()
{
    // Terms
    QSqlQueryModel *termModel = new QSqlQueryModel();
    dbHandler.openDB();
    termModel->setQuery(dbHandler.doAction("SELECT * FROM arguments"));
    ui->tvwCopyrightTerms->setModel(termModel);
    dbHandler.closeDB();

    ui->tvwCopyrightTerms->resizeColumnsToContents();

    // Paths
    QSqlQueryModel *pathModel = new QSqlQueryModel();
    dbHandler.openDB();
    pathModel->setQuery(dbHandler.doAction("SELECT * FROM paths"));
    ui->tvwPathPreferences->setModel(pathModel);
    dbHandler.closeDB();

    ui->tvwPathPreferences->resizeColumnsToContents();
}

void wDBCopyrights::on_btnCreateBackup_clicked()
{
    dbHandler.createBackup();
}


void wDBCopyrights::on_actionClose_triggered()
{
    close();
}


void wDBCopyrights::on_actionBackToHome_triggered()
{
    close();
    backToHome();
}

