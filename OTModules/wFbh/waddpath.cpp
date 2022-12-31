#include "waddpath.h"
#include "ui_waddpath.h"

wAddPath::wAddPath(bool isNewEntry, QWidget *parent, int ID, QString path, QString argumentIDs, int redirect) :
    QMainWindow(parent),
    ui(new Ui::wAddPath)
{
    qDebug().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle("Add / edit path");

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    dbHandler.dbPath = dbPath;
    dbHandler.setupDatabase();
    qInfo() << dbHandler.openDB();

    QSqlQueryModel *termModel = new QSqlQueryModel();
    termModel->setQuery(dbHandler.doAction("SELECT * FROM arguments"));
    ui->tvwTerms->setModel(termModel);
    dbHandler.closeDB();

    ui->tvwTerms->resizeColumnsToContents();

    dbHandler.openDB();
    QSqlQuery pathIDQuery = dbHandler.doAction("SELECT ID FROM paths");
    QStringList pathIDList;

    while (pathIDQuery.next())
        pathIDList << pathIDQuery.value(0).toString();

    ui->cobxRedirect->addItems(pathIDList);

    dbHandler.closeDB();

    if (!isNewEntry)
    {
        ui->ledID->setText(QString::number(ID));
        ui->ledPath->setText(path);

        if (redirect == -1)
            ui->cbxRedirect->setChecked(false);
        else
            ui->cbxRedirect->setChecked(true);

        QStringList argumentIDList = argumentIDs.split("|");
        argumentIDList.removeAll("");
        qInfo() << argumentIDList;


        QItemSelectionModel *model = ui->tvwTerms->selectionModel();

        for (int i = 0; i < ui->tvwTerms->model()->rowCount(); i++)
            if (argumentIDList.contains(ui->tvwTerms->model()->index(i, 0).data().toString()))
                    model->select(ui->tvwTerms->model()->index(i, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);

        ui->tvwTerms->setSelectionModel(model);
    }

    on_cbxRedirect_stateChanged(0);
    on_ledPath_textChanged(ui->ledPath->text());
    ui->lblPath->setFocus();

    qDebug().noquote() << objectName() + " started";
}

wAddPath::~wAddPath()
{
    delete ui;
}

void wAddPath::on_cbxRedirect_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    ui->cobxRedirect->setEnabled(ui->cbxRedirect->isChecked());
    ui->tvwTerms->setEnabled(!ui->cbxRedirect->isChecked());
}


void wAddPath::on_btnSave_clicked()
{
    QString argumentIDs = "|";

    for (int i = 0; i < ui->tvwTerms->selectionModel()->selectedRows().count(); i++)
        argumentIDs += ui->tvwTerms->selectionModel()->selectedRows().at(i).data().toString() + "|";

    int redirect = -1;
    if (ui->cbxRedirect->isChecked())
        redirect = ui->cobxRedirect->currentText().toInt();

    if (ui->ledID->text().isEmpty())
        dialogFinished(-1, ui->ledPath->text(), argumentIDs, redirect);
    else
        dialogFinished(ui->ledID->text().toInt(), ui->ledPath->text(), argumentIDs, redirect);

    close();
}


void wAddPath::on_ledPath_textChanged(const QString &arg1)
{
    ui->btnSave->setEnabled(!arg1.isEmpty());
}

