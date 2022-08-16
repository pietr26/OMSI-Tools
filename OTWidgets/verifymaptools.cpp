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

/// Copies missing objects
void verifyMapTools::copy(QListWidget *lwg)
{
    if (lwg->count() == 0)
        return;
    else if (lwg->selectedItems().count() == 0)
        lwg->selectAll();

    QString copytext = "";
    foreach (QListWidgetItem* current, lwg->selectedItems())
        copytext += current->text() + "\n";

    misc.copy(copytext);
}

/// Ignores the selected path(s)
void verifyMapTools::ignore(QListWidget *lwg)
{
    if (lwg->count() == 0)
        return;
    else if (lwg->selectedItems().count() == 0)
        lwg->selectAll();

    foreach (QListWidgetItem* current, lwg->selectedItems())
        iglF.write(current->text());

    qDeleteAll(lwg->selectedItems());
}

/// Search for paths in wContentSearch
void verifyMapTools::search(QListWidget *lwg)
{
    if (lwg->count() == 0)
        return;
    else if (lwg->selectedItems().count() == 0)
        lwg->selectAll();

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

/// Shows menu for copy button
void verifyMapTools::on_tbnCopy_clicked()
{
    ui->tbnCopy->showMenu();
}

/// Shows menu for search button
void verifyMapTools::on_tbnSearch_clicked()
{
    ui->tbnSearch->showMenu();
}

/// Shows menu for ignore button
void verifyMapTools::on_tbnIgnore_clicked()
{
    ui->tbnIgnore->showMenu();
}

/// Copies all file paths
void verifyMapTools::on_actionCopyFromAll_triggered()
{
    copy(listWidgetAllParent);
}

/// Copies missing file paths
void verifyMapTools::on_actionCopyFromMissing_triggered()
{
    copy(listWidgetMissingParent);
}

/// Searchs all file paths
void verifyMapTools::on_actionSearchFromAll_triggered()
{
    search(listWidgetAllParent);
}

/// Searchs missing file paths
void verifyMapTools::on_actionSearchFromMissing_triggered()
{
    search(listWidgetMissingParent);
}

/// Ignores all file paths
void verifyMapTools::on_actionIgnoreFromAll_triggered()
{
    ignore(listWidgetAllParent);
}

/// Ignores missing file paths
void verifyMapTools::on_actionIgnoreFromMissing_triggered()
{
    ignore(listWidgetMissingParent);
}
