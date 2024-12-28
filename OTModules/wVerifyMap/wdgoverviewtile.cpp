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

void wdgOverviewTile::setCount(int count)
{
    ui->lblTotal->setText(QString::number(count));
    if (count == 0) ui->lblIcon->setPixmap(QPixmap());
}

void wdgOverviewTile::setMissing(int missing)
{
    ui->lblMissing->setText(QString::number(missing) + " " + tr("missing", "x items missing"));

    if (missing == 0)
    {
        ui->lblIcon->setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::WeatherClear).pixmap(32));
        ui->btnTo->setVisible(false);
    }
    else
    {
        ui->btnTo->setVisible(true);

        if (warningInsteadError)
            ui->lblIcon->setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::DialogWarning).pixmap(32));
        else
            ui->lblIcon->setPixmap(QIcon::fromTheme(QIcon::ThemeIcon::EditClear).pixmap(32));
    }
}

void wdgOverviewTile::clear()
{
    ui->lblTotal->setText("0");
    ui->lblMissing->setText("0 " + tr("missing", "x items missing"));
    ui->btnTo->setVisible(false);
    QPixmap empty = QPixmap(32, 32);
    empty.fill(Qt::transparent);
    ui->lblIcon->setPixmap(empty);
}

void wdgOverviewTile::setName(QString name) { ui->lblName->setText(name); }
void wdgOverviewTile::setWarningInsteadError(bool enable) { warningInsteadError = enable; }

void wdgOverviewTile::on_btnTo_clicked() { goTo(); }
