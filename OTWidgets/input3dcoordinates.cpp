#include "input3dcoordinates.h"
#include "ui_input3dcoordinates.h"

input3DCoordinates::input3DCoordinates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::input3DCoordinates)
{
    ui->setupUi(this);
}

input3DCoordinates::~input3DCoordinates()
{
    delete ui;
}

float input3DCoordinates::getValue1() { return ui->sbx1->value(); }

void input3DCoordinates::setValue1(float value) { ui->sbx1->setValue(value); }

float input3DCoordinates::getValue2() { return ui->sbx2->value(); }

void input3DCoordinates::setValue2(float value) { ui->sbx2->setValue(value); }

float input3DCoordinates::getValue3() { return ui->sbx3->value(); }

void input3DCoordinates::setValue3(float value) { ui->sbx3->setValue(value); }

void input3DCoordinates::setSuffix(QString suffix)
{
    ui->sbx1->setSuffix(suffix);
    ui->sbx2->setSuffix(suffix);
    ui->sbx3->setSuffix(suffix);
}

void input3DCoordinates::setDecimals(int decimals)
{
    ui->sbx1->setDecimals(decimals);
    ui->sbx2->setDecimals(decimals);
    ui->sbx3->setDecimals(decimals);
}

void input3DCoordinates::setMin(double min)
{
    ui->sbx1->setMinimum(min);
    ui->sbx2->setMinimum(min);
    ui->sbx3->setMinimum(min);
}

void input3DCoordinates::setMax(double max)
{
    ui->sbx1->setMaximum(max);
    ui->sbx2->setMaximum(max);
    ui->sbx3->setMaximum(max);
}

void input3DCoordinates::setName1(QString name1)
{
    ui->lbl1->setVisible(!name1.isEmpty());
    ui->lbl1->setText(name1);
}

void input3DCoordinates::setName2(QString name2)
{
    ui->lbl2->setVisible(!name2.isEmpty());
    ui->lbl2->setText(name2);
}

void input3DCoordinates::setName3(QString name3)
{
    ui->lbl3->setVisible(!name3.isEmpty());
    ui->lbl3->setText(name3);
}

QString input3DCoordinates::getSuffix() { return ui->sbx1->suffix(); }

int input3DCoordinates::getDecimals() { return ui->sbx1->decimals(); }

double input3DCoordinates::getMin() { return ui->sbx1->minimum(); }

double input3DCoordinates::getMax() { return ui->sbx1->maximum(); }

QString input3DCoordinates::getName1() { return ui->lbl1->text(); }

QString input3DCoordinates::getName2() { return ui->lbl2->text(); }

QString input3DCoordinates::getName3() { return ui->lbl3->text(); }
