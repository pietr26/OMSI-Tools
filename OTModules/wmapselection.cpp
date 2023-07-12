#include "wmapselection.h"
#include "ui_wmapselection.h"

wMapSelection::wMapSelection(QWidget *parent, QString lastMapPath, bool withPath) :
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

    loadList(withPath);

    ui->lwgMaps->setCurrentRow(-1);

    if (lastMapPath != "")
    {
        for (int i = 0; i < ui->lwgMaps->count(); i++)
        {
            if (maps[i].second == lastMapPath)
            {
                ui->lwgMaps->setCurrentRow(i);
                break;
            }
        }
    }
}

wMapSelection::~wMapSelection()
{
    delete ui;
}

/// Loads the map list
void wMapSelection::loadList(bool withPath)
{
    ui->lwgMaps->clear();
    maps = filehandler.listMaps();
    qDebug() << "Map count:" << maps.size();

    for (int i = 0; i < maps.size(); i++)
    {
        QString pathPart = (withPath) ? " (" + maps[i].second + ")" : "";
        ui->lwgMaps->addItem(maps[i].first + pathPart);
    }
}

/// Applies the selected map
void wMapSelection::on_btnConfirm_clicked()
{
    if (ui->lwgMaps->selectedItems().size() == 0)
        return;

    QPair<QString, QString> pair = maps[ui->lwgMaps->currentRow()];
    returnMapInfo(pair);
    close();
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

