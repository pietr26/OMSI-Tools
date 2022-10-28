#include "message.h"
#include "ui_message.h"

message::message(OTInAppMessage paramMessageData, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::message)
{
    ui->setupUi(this);
    messageData = paramMessageData;

    if (set.read("main", "language") == "1")
    {
        ui->lblTitle->setText(messageData.deTitle);
        ui->lblShortDescription->setText(messageData.deShortDescription);
    }
    else
    {
        ui->lblTitle->setText(messageData.enTitle);
        ui->lblShortDescription->setText(messageData.enShortDescription);
    }

    if (set.read("main\\messages", messageData.ID).toBool())
        ui->lblUnreadPoint->hide();
}

message::~message()
{
    delete ui;
}

void message::showDescription()
{
    set.write("main\\messages", messageData.ID, true);
    ui->lblUnreadPoint->hide();

    WMESSAGEVIEWER = new wMessageViewer(messageData, this->parentWidget());
    WMESSAGEVIEWER->setWindowModality(Qt::ApplicationModal);
    WMESSAGEVIEWER->show();
}

