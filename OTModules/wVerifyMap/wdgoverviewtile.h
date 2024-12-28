#ifndef WDGOVERVIEWTILE_H
#define WDGOVERVIEWTILE_H

#include <QWidget>

namespace Ui {
class wdgOverviewTile;
}

class wdgOverviewTile : public QWidget
{
    Q_OBJECT

public:
    explicit wdgOverviewTile(QWidget *parent = nullptr);
    ~wdgOverviewTile();

    void setCount(int count);
    void setMissing(int missing);
    void clear();
    void setWarningInsteadError(bool enable);
    void setName(QString name);

signals:
    void goTo();

private slots:
    void on_btnTo_clicked();

private:
    Ui::wdgOverviewTile *ui;

    bool warningInsteadError = false;
};

#endif // WDGOVERVIEWTILE_H
