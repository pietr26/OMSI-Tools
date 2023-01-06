#include "wselectencoding.h"
#include "ui_wselectencoding.h"

wSelectEncoding::wSelectEncoding(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wSelectEncoding)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    qDebug() << "UI set";

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    // Load settings
    setStyleSheet(set.read("main","theme").toString());
    setWindowTitle(tr("Select encoding..."));


    qInfo().noquote() << objectName() + " started";
}

wSelectEncoding::~wSelectEncoding()
{
    delete ui;
}

void wSelectEncoding::on_btnSelect_clicked()
{
    switch (ui->cobxEncoding->currentIndex())
    {
        case 0: encodingSelected(QStringConverter::Latin1); break;
        case 1: encodingSelected(QStringConverter::Utf8); break;
        case 2: encodingSelected(QStringConverter::Utf16); break;
        case 3: encodingSelected(QStringConverter::Utf16LE); break;
        case 4: encodingSelected(QStringConverter::Utf16BE); break;
        case 5: encodingSelected(QStringConverter::Utf32); break;
        case 6: encodingSelected(QStringConverter::Utf32LE); break;
        case 7: encodingSelected(QStringConverter::Utf32BE); break;
    }

    close();
}
