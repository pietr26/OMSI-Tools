#include "wdgoverviewtile.h"
#include "ui_wdgoverviewtile.h"

wdgOverviewTile::wdgOverviewTile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgOverviewTile)
{
    ui->setupUi(this);
    clear();
}

wdgOverviewTile::~wdgOverviewTile()
{
    delete ui;
}

void wdgOverviewTile::setTotal(int count)
{
    switch (type)
    {
        case Tile: ui->lblTotal->setText(tr("%n tile(s)", "", count)); break;
        case Sceneryobject: ui->lblTotal->setText(tr("%n sceneryobjec(ts)", "", count)); break;
        case Spline: ui->lblTotal->setText(tr("%n spline(s)", "", count)); break;
        case Vehicle: ui->lblTotal->setText(tr("%n vehicle(s)", "", count)); break;
        case Human: ui->lblTotal->setText(tr("%n human(s)", "", count)); break;
        case Texture: ui->lblTotal->setText(tr("%n texture(s)", "", count)); break;
        case None: ui->lblTotal->setText(tr("%n element(s)", "", count)); break;
    }

    if (count == 0) ui->lblIcon->setPixmap(QPixmap());
}

void wdgOverviewTile::setMissing(int missing)
{
    ui->lblMissing->setText(tr("%n missing", "", missing));

    ui->btnTo->setVisible(missing != 0);

    if (missing == 0)  ui->lblIcon->setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::WeatherClear).pixmap(32));
    else
    {
        if (warningInsteadError) ui->lblIcon->setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::DialogWarning).pixmap(32));
        else ui->lblIcon->setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::EditClear).pixmap(32));
    }
}

void wdgOverviewTile::clear()
{
    setMissing(0);
    setTotal(0);
    ui->btnTo->setVisible(false);
    QPixmap empty = QPixmap(32, 32);
    empty.fill(Qt::transparent);
    ui->lblIcon->setPixmap(empty);
}

void wdgOverviewTile::setElementType(ElementType _type)
{
    type = _type; clear();
}

void wdgOverviewTile::setWarningInsteadError(bool enable) { warningInsteadError = enable; }

void wdgOverviewTile::on_btnTo_clicked() { goTo(); }
