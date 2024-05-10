#include "waddword.h"
#include "ui_waddword.h"

wAddWord::wAddWord(bool isNewEntry, QWidget *parent, int ID, QString word) :
    QMainWindow(parent),
    ui(new Ui::wAddWord)
{
    qInfo().noquote() << "Starting " + objectName() + "...";
    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowTitle("Add word");

    if (!isNewEntry)
    {
        ui->ledID->setText(QString::number(ID));
        ui->ledWord->setText(word);
    }

    on_ledWord_textChanged(ui->ledWord->text());
    ui->ledWord->setFocus();

    qInfo().noquote() << objectName() + " started";
}

wAddWord::~wAddWord()
{
    delete ui;
}

void wAddWord::on_btnSave_clicked()
{
    if (ui->ledID->text().isEmpty())
        emit dialogFinished(-1, ui->ledWord->text());
    else
        emit dialogFinished(ui->ledID->text().toInt(), ui->ledWord->text());

    close();
}


void wAddWord::on_ledWord_textChanged(const QString &arg1)
{
    ui->btnSave->setEnabled(!arg1.isEmpty());
}

