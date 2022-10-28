#include "wmessageviewer.h"
#include "ui_wmessageviewer.h"

wMessageViewer::wMessageViewer(OTInAppMessage messageData, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wMessageViewer)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog);

    if (set.read("main", "language") == "1")
    {
        setWindowTitle(tr("Message: %1").arg(messageData.deTitle));
        ui->lblDescription->setText(messageData.deDescription);
    }
    else
    {
        setWindowTitle(tr("Message: %1").arg(messageData.enTitle));
        ui->lblDescription->setText(messageData.enDescription);
    }

    ui->lblPublishDate->setText(tr("Published on %1", "e.g. Published on 28.10.2022").arg(messageData.start.toString("dd.MM.yyyy, HH:mm")));
}

wMessageViewer::~wMessageViewer()
{
    delete ui;
}
