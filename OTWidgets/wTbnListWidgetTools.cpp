#include "wTbnListWidgetTools.h"
#include "ui_wdlwgtools.h"

wdLwgTools::wdLwgTools(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wdLwgTools)
{
    ui->setupUi(this);
}

wdLwgTools::~wdLwgTools()
{
    delete ui;
}
