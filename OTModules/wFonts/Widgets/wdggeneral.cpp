#include "wdggeneral.h"
#include "ui_wdggeneral.h"

wdgGeneral::wdgGeneral(QWidget *parent, OTFontModel &font)
    : QWidget(parent)
    , ui(new Ui::wdgGeneral)
{
    ui->setupUi(this);
}

wdgGeneral::~wdgGeneral()
{
    delete ui;
}
