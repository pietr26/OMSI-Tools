#include "input2dcoordinates.h"
#include "ui_input2dcoordinates.h"

input2DCoordinates::input2DCoordinates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::input2DCoordinates)
{
    ui->setupUi(this);
}

input2DCoordinates::~input2DCoordinates()
{
    delete ui;
}

float input2DCoordinates::getValue1() { return ui->sbx1->value(); }

void input2DCoordinates::setValue1(float value) { ui->sbx1->setValue(value); }

float input2DCoordinates::getValue2() { return ui->sbx2->value(); }

void input2DCoordinates::setValue2(float value) { ui->sbx2->setValue(value); }

void input2DCoordinates::setSuffix(QString suffix)
{
    ui->sbx1->setSuffix(suffix);
    ui->sbx2->setSuffix(suffix);
}

void input2DCoordinates::setDecimals(int decimals)
{
    ui->sbx1->setDecimals(decimals);
    ui->sbx2->setDecimals(decimals);
}

void input2DCoordinates::setMin(double min)
{
    ui->sbx1->setMinimum(min);
    ui->sbx2->setMinimum(min);
}

void input2DCoordinates::setMax(double max)
{
    ui->sbx1->setMaximum(max);
    ui->sbx2->setMaximum(max);
}

void input2DCoordinates::setName1(QString name1)
{
    ui->lbl1->setVisible(!name1.isEmpty());
    ui->lbl1->setText(name1);
}

void input2DCoordinates::setName2(QString name2)
{
    ui->lbl2->setVisible(!name2.isEmpty());
    ui->lbl2->setText(name2);
}

QString input2DCoordinates::getSuffix() { return ui->sbx1->suffix(); }

int input2DCoordinates::getDecimals() { return ui->sbx1->decimals(); }

double input2DCoordinates::getMin() { return ui->sbx1->minimum(); }

double input2DCoordinates::getMax() { return ui->sbx1->maximum(); }

QString input2DCoordinates::getName1() { return ui->lbl1->text(); }

QString input2DCoordinates::getName2() { return ui->lbl2->text(); }
