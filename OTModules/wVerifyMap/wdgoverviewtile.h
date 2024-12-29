#ifndef WDGOVERVIEWTILE_H
#define WDGOVERVIEWTILE_H

#include <QWidget>

namespace Ui {
class wdgOverviewTile;
}

enum ElementType
{
    None = 0,
    Tile = 1,
    Sceneryobject = 2,
    Spline = 3,
    Vehicle = 4,
    Human = 5,
    Texture = 6
};

class wdgOverviewTile : public QWidget
{
    Q_OBJECT

public:
    explicit wdgOverviewTile(QWidget *parent = nullptr);
    ~wdgOverviewTile();

    void setTotal(int count);
    void setMissing(int missing);
    void clear();
    void setWarningInsteadError(bool enable);
    void setElementType(ElementType _type);

signals:
    void goTo();

private slots:
    void on_btnTo_clicked();

private:
    Ui::wdgOverviewTile *ui;

    bool warningInsteadError = false;

    ElementType type;
};

#endif // WDGOVERVIEWTILE_H
