#include "verifymaptools.h"
#include "ui_verifymaptools.h"

verifyMapTools::verifyMapTools(QListWidget *lwgAllParent, QListWidget *lwgMissingParent, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::verifyMapTools)
{
    ui->setupUi(this);
    listWidgetAllParent = lwgAllParent;
    listWidgetMissingParent = lwgMissingParent;

    ui->tbnCopy->addAction(ui->actionCopyFromAll);
    ui->tbnCopy->addAction(ui->actionCopyFromMissing);

    ui->tbnSearch->addAction(ui->actionSearchFromAll);
    ui->tbnSearch->addAction(ui->actionSearchFromMissing);

    ui->tbnIgnore->addAction(ui->actionIgnoreFromAll);
    ui->tbnIgnore->addAction(ui->actionIgnoreFromMissing);
}

verifyMapTools::~verifyMapTools()
{
    delete ui;
}

/// \brief Copies missing objects
void verifyMapTools::copy(QListWidget *lwg)
{
    if (lwg->currentRow() == -1)
        return;

    QString copytext = "";
    foreach (QListWidgetItem* current, lwg->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
}

/// \brief Ignores the selected path(s)
void verifyMapTools::ignore(QListWidget *lwg)
{
    if (lwg->currentRow() == -1)
        return;

    for (int i = 0; i < lwg->count(); i++)
        if (lwg->item(i)->isSelected())
            iglF.write(lwg->item(i)->text());

    qDeleteAll(lwg->selectedItems());
}

/// \brief Search for paths in wContentSearch
void verifyMapTools::search(QListWidget *lwg)
{
    QStringList paths;
    foreach(QListWidgetItem* current, lwg->selectedItems())
        paths << current->text();

    if (!paths.empty())
    {
        WCONTENTSEARCH = new wContentSearch(this, paths);
        WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
        WCONTENTSEARCH->show();
    }
}


void verifyMapTools::on_tbnCopy_clicked()
{
    ui->tbnCopy->showMenu();
}


void verifyMapTools::on_tbnSearch_clicked()
{
    ui->tbnSearch->showMenu();
}


void verifyMapTools::on_tbnIgnore_clicked()
{
    ui->tbnIgnore->showMenu();
}


void verifyMapTools::on_actionCopyFromAll_triggered()
{
    copy(listWidgetAllParent);
}


void verifyMapTools::on_actionCopyFromMissing_triggered()
{
    copy(listWidgetMissingParent);
}


void verifyMapTools::on_actionSearchFromAll_triggered()
{
    search(listWidgetAllParent);
}


void verifyMapTools::on_actionSearchFromMissing_triggered()
{
    search(listWidgetMissingParent);
}


void verifyMapTools::on_actionIgnoreFromAll_triggered()
{
    ignore(listWidgetAllParent);
}


void verifyMapTools::on_actionIgnoreFromMissing_triggered()
{
    ignore(listWidgetMissingParent);
}

