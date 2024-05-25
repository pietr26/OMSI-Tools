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

    // Load prefs
    setWindowTitle(tr("Select encoding"));


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
        case 0: emit encodingSelected(QStringConverter::Latin1); break;
        case 1: emit encodingSelected(QStringConverter::Utf8); break;
        case 2: emit encodingSelected(QStringConverter::Utf16); break;
        case 3: emit encodingSelected(QStringConverter::Utf16LE); break;
        case 4: emit encodingSelected(QStringConverter::Utf16BE); break;
        case 5: emit encodingSelected(QStringConverter::Utf32); break;
        case 6: emit encodingSelected(QStringConverter::Utf32LE); break;
        case 7: emit encodingSelected(QStringConverter::Utf32BE); break;
    }

    close();
}
