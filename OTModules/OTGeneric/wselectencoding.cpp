#include "wselectencoding.h"
#include "ui_wselectencoding.h"

#include <QDebug>

wSelectEncoding::wSelectEncoding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wSelectEncoding)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    qDebug() << "UI set";

    setWindowTitle(tr("Select encoding"));

    qInfo().noquote() << objectName() + " started";
}

wSelectEncoding::~wSelectEncoding()
{
    delete ui;
}

QStringConverter::Encoding wSelectEncoding::selectedEncoding() const
{
    switch (ui->cobxEncoding->currentIndex())
    {
        case 1:  return QStringConverter::Utf8;
        case 2:  return QStringConverter::Utf16;
        case 3:  return QStringConverter::Utf16LE;
        case 4:  return QStringConverter::Utf16BE;
        case 5:  return QStringConverter::Utf32;
        case 6:  return QStringConverter::Utf32LE;
        case 7:  return QStringConverter::Utf32BE;
        default: return QStringConverter::Latin1;
    }
}

void wSelectEncoding::on_btnSelect_clicked()
{
    accept();
}
