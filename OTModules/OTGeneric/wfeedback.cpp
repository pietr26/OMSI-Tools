#include "wfeedback.h"
#include "ui_wfeedback.h"

wFeedback::wFeedback(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wFeedback)
{
    ui->setupUi(this);
}

wFeedback::~wFeedback()
{
    delete ui;
}
