#include "wdgchars.h"
#include "ui_wdgchars.h"

wdgChars::wdgChars(QWidget *parent, OTFontModel &font)
    : QWidget(parent)
    , ui(new Ui::wdgChars)
{
    ui->setupUi(this);
}

wdgChars::~wdgChars()
{
    delete ui;
}
