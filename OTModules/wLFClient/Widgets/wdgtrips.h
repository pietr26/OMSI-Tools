#ifndef WDGTRIPS_H
#define WDGTRIPS_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class wdgTrips;
}

class wdgTrips : public QWidget
{
    Q_OBJECT

public:
    explicit wdgTrips(QWidget *parent = nullptr);
    ~wdgTrips();

private:
    Ui::wdgTrips *ui;

    QStandardItemModel *trips;
};

#endif // WDGTRIPS_H
