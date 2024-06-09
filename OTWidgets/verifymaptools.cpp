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

void verifyMapTools::on_tbnCopy_clicked() { ui->tbnCopy->showMenu(); }

void verifyMapTools::on_tbnSearch_clicked() { ui->tbnSearch->showMenu(); }

void verifyMapTools::on_actionCopyFromAll_triggered() { copy(listWidgetAllParent); }

void verifyMapTools::on_actionCopyFromMissing_triggered() { copy(listWidgetMissingParent); }

void verifyMapTools::on_actionSearchFromAll_triggered() { search(listWidgetAllParent); }

void verifyMapTools::on_actionSearchFromMissing_triggered() { search(listWidgetMissingParent); }
