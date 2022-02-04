#include "verifymaptools.h"
#include "ui_verifymaptools.h"

verifyMapTools::verifyMapTools(QListWidget *lwgParent, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::verifyMapTools)
{
    ui->setupUi(this);
    listWidgetParent = lwgParent;
}

verifyMapTools::~verifyMapTools()
{
    delete ui;
}

void verifyMapTools::on_tbnTilesTools_clicked()
{
    ui->tbnMain->showMenu();
}

/// \brief Copies missing objects
void verifyMapTools::copy()
{
    if (listWidgetParent->currentRow() == -1)
        return;

    QString copytext = "";
    foreach (QListWidgetItem* current, listWidgetParent->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
}

/// \brief Ignores the selected path(s)
void verifyMapTools::ignore()
{
    if (listWidgetParent->currentRow() == -1)
        return;

    for (int i = 0; i < listWidgetParent->count(); i++)
    {
        if (listWidgetParent->item(i)->isSelected())
        {
            iglF.write(listWidgetParent->item(i)->text());
        }
    }
    qDeleteAll(listWidgetParent->selectedItems());
}

/// \brief Search for paths in wContentSearch
void verifyMapTools::search()
{
    QStringList paths;
    foreach(QListWidgetItem* current, listWidgetParent->selectedItems())
        paths << set.read("main", "mainDir").toString() + "/" + current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
}

