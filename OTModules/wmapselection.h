#ifndef WMAPSELECTION_H
#define WMAPSELECTION_H

#include <QMainWindow>
#include <OTBackend/OTGlobal.h>
#include <OTBackend/OTOmsiFileHandler.h>
#include <QListWidgetItem>

namespace Ui {
class wMapSelection;
}

class wMapSelection : public QMainWindow
{
    Q_OBJECT

public:
    explicit wMapSelection(QWidget *parent = nullptr, QString lastMap = "", bool withPath = false);
    ~wMapSelection();

signals:
    void returnMapInfo(QPair<QString, QString> mapInfo);

private slots:
    void on_btnConfirm_clicked();

    void on_btnReloadList_clicked();

    void on_lwgMaps_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::wMapSelection *ui;
    OTOMSIFileHandler filehandler;
    OTSettings set;
    OTMiscellaneous misc;

    void loadList(bool withPath = false);

    QList<QPair<QString, QString>> maps;
};

#endif // WMAPSELECTION_H
