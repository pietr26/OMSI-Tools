#include "wbugdoc.h"
#include "ui_wbugdoc.h"

wBugDoc::wBugDoc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wBugDoc)
{
    ui->setupUi(this);
}

wBugDoc::~wBugDoc()
{
    delete ui;
}
