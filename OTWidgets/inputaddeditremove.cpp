#include "inputaddeditremove.h"
#include "ui_inputaddeditremove.h"

inputAddEditRemove::inputAddEditRemove(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputAddEditRemove)
{
    ui->setupUi(this);
}

inputAddEditRemove::~inputAddEditRemove()
{
    delete ui;
}

void inputAddEditRemove::setAddEnabled(bool enabled) { ui->btnAdd->setEnabled(enabled); }

void inputAddEditRemove::setEditEnabled(bool enabled) { ui->btnEdit->setEnabled(enabled); }

void inputAddEditRemove::setRemoveEnabled(bool enabled) { ui->btnRemove->setEnabled(enabled); }

void inputAddEditRemove::on_btnAdd_clicked() { emit addClicked(); }

void inputAddEditRemove::on_btnEdit_clicked() { emit editClicked(); }

void inputAddEditRemove::on_btnRemove_clicked() { emit removeClicked(); }
