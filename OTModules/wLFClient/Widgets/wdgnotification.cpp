#include "wdgnotification.h"
#include "ui_wdgnotification.h"

wdgNotification::wdgNotification(QStringList lines, QString title, QString text, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgNotification)
{
    ui->setupUi(this);

    ui->lblNotification->setText("# [" + lines.join(", ") + "] " + title + "\n" + text);
}

wdgNotification::~wdgNotification()
{
    delete ui;
}
