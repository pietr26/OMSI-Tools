#include "waddterm.h"
#include "ui_waddterm.h"

wAddTerm::wAddTerm(bool isNewEntry, QWidget *parent, int ID, QString argument) :
    QMainWindow(parent),
    ui(new Ui::wAddTerm)
{
    qDebug().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle("Add / edit term");

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    if (!isNewEntry)
    {
        ui->ledID->setText(QString::number(ID));
        ui->pteArgument->setPlainText(argument);
    }

    on_pteArgument_textChanged();
    ui->pteArgument->setFocus();

    qDebug().noquote() << objectName() + " started";
}

wAddTerm::~wAddTerm()
{
    delete ui;
}

void wAddTerm::on_btnSave_clicked()
{
    if (ui->ledID->text().isEmpty())
        emit dialogFinished(-1, ui->pteArgument->toPlainText());
    else
        emit dialogFinished(ui->ledID->text().toInt(), ui->pteArgument->toPlainText());

    close();
}


void wAddTerm::on_pteArgument_textChanged()
{
    ui->btnSave->setEnabled(!ui->pteArgument->toPlainText().isEmpty());
}
