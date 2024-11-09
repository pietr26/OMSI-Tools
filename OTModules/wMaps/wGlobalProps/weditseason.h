#ifndef WEDITSEASON_H
#define WEDITSEASON_H

#include <QMainWindow>
#include "OTBackend/OCC/OCMap.h"
#include <QStandardItemModel>

namespace Ui {
class wEditSeason;
}

class wEditSeason : public QMainWindow
{
    Q_OBJECT

public:
    explicit wEditSeason(OCMap::Map::Global::Season s, int index, QWidget *parent = nullptr);
    ~wEditSeason();

private slots:
    void on_btnSave_clicked();

    void on_btnDiscard_clicked();

    void on_cobxSeason_currentIndexChanged(int index);

    void on_daeStart_userDateChanged(const QDate &date);

    void on_daeEnd_userDateChanged(const QDate &date);

signals:
    void returnSeason(OCMap::Map::Global::Season season);

private:
    Ui::wEditSeason *ui;
    OTSettings set;

    void reloadUI();

    OCMap::Map::Global::Season season = OCMap::Map::Global::Season();
    int seasonIndex;

};

#endif // WEDITSEASON_H
