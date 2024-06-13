#include "wdgpreview.h"
#include "ui_wdgpreview.h"

wdgPreview::wdgPreview(QWidget *parent, OTFontModel &font)
    : QWidget(parent)
    , ui(new Ui::wdgPreview)
{
    ui->setupUi(this);
}

wdgPreview::~wdgPreview()
{
    delete ui;
}
