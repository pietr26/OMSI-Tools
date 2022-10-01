#include "wmapselection.h"
#include "ui_wmapselection.h"

wMapSelection::wMapSelection(QWidget *parent, QString lastMap) :
    QMainWindow(parent),
    ui(new Ui::wMapSelection)
{
    ui->setupUi(this);
    ui->statusbar->setVisible(false);
    adjustSize();
    resize(misc.sizeWindow(0.25, 0.6));

    setWindowTitle(tr("Select map..."));
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    ui->btnConfirm->setShortcut(Qt::Key_Return | Qt::Key_Enter);

    loadList();

    qDebug() << lastMap.remove(0, set.read("main", "mainDir").toString().size() + 1).remove(QRegularExpression("/global.cfg"));
    for (int i = 0; i < ui->lwgMaps->count(); i++)
    {
        if (ui->lwgMaps->item(i)->text().contains(lastMap.remove(0, set.read("main", "mainDir").toString().size() + 1).remove(QRegularExpression("/global.cfg"))))
            ui->lwgMaps->setCurrentRow(i);
    }
}

wMapSelection::~wMapSelection()
{
    delete ui;
}

/// Loads the map list
void wMapSelection::loadList()
{
    ui->lwgMaps->clear();
    maps = filehandler.listMaps();
    qDebug() << "Map count:" << maps.size();

    for (int i = 0; i < maps.size(); i++)
        ui->lwgMaps->addItem(maps[i].second + " (" + maps[i].first + ")");
}

/// Applies the selected map
void wMapSelection::on_btnConfirm_clicked()
{
    if (ui->lwgMaps->selectedItems().size() == 0)
        return;

    QPair<QString, QString> pair = maps[ui->lwgMaps->currentRow()];
    pair.second = set.read("main", "mainDir").toString() + "/" + pair.first + "/global.cfg";
    returnMapInfo(pair);
}

/// Reloads the list (button)
void wMapSelection::on_btnReloadList_clicked()
{
    loadList();
}

/// Wrapped to on_btnConfirm_clicked()
void wMapSelection::on_lwgMaps_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    on_btnConfirm_clicked();
}

