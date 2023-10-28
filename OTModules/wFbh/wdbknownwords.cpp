#include "wdbknownwords.h"
#include "ui_wdbknownwords.h"

wDBKnownWords::wDBKnownWords(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wDBKnownWords)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle(OTInformation::name + " - known words panel");
    resize(misc.sizeWindow(0.1, 0.8));

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    // Setup database
    dbHandler.dbPath = "D:/OMSI-Tools/OMSI-Tools/data/db/webdisk-tools.db";
    if (!QFile(QDir(dbHandler.dbPath).absolutePath()).exists())
    {
        dbHandler.setupDatabase(true);
        dbHandler.doAction("CREATE TABLE 'knownWords' ('ID' INTEGER, 'word' TEXT, PRIMARY KEY('ID' AUTOINCREMENT))", true);
    }
    else
        dbHandler.setupDatabase();

    updateView();

    qInfo().noquote() << objectName() + " started";
}

wDBKnownWords::~wDBKnownWords()
{
    delete ui;
}

void wDBKnownWords::on_actionBackToHome_triggered()
{
    emit backToHome();
    close();
}


void wDBKnownWords::on_actionClose_triggered()
{
    close();
}

void wDBKnownWords::updateView()
{
    // Terms
    QSqlQueryModel *termModel = new QSqlQueryModel();
    dbHandler.openDB();
    termModel->setQuery(dbHandler.doAction("SELECT * FROM knownWords"));
    ui->tvwWords->setModel(termModel);
    dbHandler.closeDB();

    ui->tvwWords->resizeColumnsToContents();

}

void wDBKnownWords::addWordFinished(int ID, QString word)
{
    if (ID == -1)
        dbHandler.doAction(QString("INSERT INTO knownWords (word) VALUES ('%1')").arg(word), true);
    else
        dbHandler.doAction(QString("UPDATE knownWords SET word = '%1' WHERE ID = %2").arg(word, QString::number(ID)), true);

    updateView();
}

void wDBKnownWords::on_btnAdd_clicked()
{
    wAddWord *addWord = new wAddWord(true, this);
    addWord->show();
    connect(addWord, &wAddWord::dialogFinished, this, &wDBKnownWords::addWordFinished);

    updateView();
}

void wDBKnownWords::on_btnRemove_clicked()
{
    QItemSelectionModel *model = ui->tvwWords->selectionModel();

    qInfo() << model->selectedRows().at(0).data().toInt();

    if (model->hasSelection()) dbHandler.doAction(QString("DELETE FROM knownWords WHERE ID = %1").arg(model->selectedRows().at(0).data().toInt()), true);

    updateView();
}

void wDBKnownWords::on_tvwWords_activated(const QModelIndex &index)
{
    Q_UNUSED(index);

    int ID = ui->tvwWords->selectionModel()->selectedRows().at(0).data().toInt();
    QString word = ui->tvwWords->selectionModel()->selectedRows(1).at(0).data().toString();

    wAddWord *addTerm = new wAddWord(false, this, ID, word);
    addTerm->show();
    connect(addTerm, &wAddWord::dialogFinished, this, &wDBKnownWords::addWordFinished);

    updateView();
}

