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
