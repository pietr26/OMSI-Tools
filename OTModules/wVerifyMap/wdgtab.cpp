#include "wdgtab.h"
#include "ui_wdgtab.h"

wdgTab::wdgTab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgTab)
{
    ui->setupUi(this);
    ui->lwgAll->installEventFilter(new EventFilterCopyElements(ui->lwgAll));
    ui->lwgMissing->installEventFilter(new EventFilterCopyElements(ui->lwgMissing));
    clear();
}

wdgTab::~wdgTab()
{
    delete ui;
}

void wdgTab::clear()
{
    existing.clear();
    missing.clear();

    ui->lwgAll->clear();
    ui->lwgMissing->clear();

    ui->twgItems->setTabText(0, tr("All (%1)").arg(0));
    ui->twgItems->setTabText(1, tr("Missing (%1)").arg(0));
}

void wdgTab::add(QStringList items, bool isMissing)
{
    isApplied = false;
    if (isMissing) missing << items;
    else existing << items;
}

void wdgTab::apply()
{
    ui->lwgMissing->clear();
    ui->lwgMissing->addItems(missing);
    ui->lwgMissing->sortItems();

    ui->lwgAll->clear();
    QStringList all = QStringList(existing + missing);
    all.removeDuplicates();
    ui->lwgAll->addItems(all);
    ui->lwgAll->sortItems();
    isApplied = true;

    ui->twgItems->setTabText(0, tr("All (%1)").arg(all.count()));
    ui->twgItems->setTabText(1, tr("Missing (%1)").arg(missing.count()));
}

void wdgTab::setName(QString name)
{
    ui->btnCopySelectedElements->setText(tr("Copy selected %1", "Copy selected elements").arg(name));
    ui->btnSearchForMissingElements->setText(tr("Search for missing %1", "Search for missing elements").arg(name));
}

OTVerificationOverviewData wdgTab::getData()
{
    if (!isApplied) qFatal() << "Cannot get data from an unapplied object!";
    return OTVerificationOverviewData(missing.count(), existing.count());
}

void wdgTab::on_btnSearchForMissingElements_clicked()
{
    if (ui->lwgMissing->selectedItems().isEmpty()) search(missing);
    else search(ui->lwgMissing->selectedItems());
}

void wdgTab::on_btnCopySelectedElements_clicked()
{
    if (ui->lwgMissing->selectedItems().isEmpty()) copy(ui->twgItems->currentIndex() ? missing : existing);
    else copy(ui->twgItems->currentIndex() ? ui->lwgMissing->selectedItems() : ui->lwgAll->selectedItems());
}

void wdgTab::copy(QList<QListWidgetItem*> items)
{
    if (items.isEmpty()) return;

    QString result;
    foreach (QListWidgetItem *current, items) result += current->text() + "\n";

    misc.copy(result);
}

void wdgTab::copy(QStringList items)
{
    if (items.isEmpty()) return;

    misc.copy(items.join("\n"));
}

void wdgTab::search(QList<QListWidgetItem*> items)
{
    if (items.isEmpty()) return;

    QStringList result;
    foreach (QListWidgetItem *current, items) result << current->text();

    WCONTENTSEARCH = new wContentSearch(this, result);
    WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
    WCONTENTSEARCH->show();
}

void wdgTab::search(QStringList items)
{
    if (items.isEmpty()) return;

    WCONTENTSEARCH = new wContentSearch(this, items);
    WCONTENTSEARCH->setWindowModality(Qt::ApplicationModal);
    WCONTENTSEARCH->show();
}

void wdgTab::setPath()
{
    if (ui->twgItems->currentIndex())
    {
        if (ui->lwgMissing->currentRow() == -1 || ui->lwgMissing->selectedItems().count() > 1) ui->ledPath->clear();
        else ui->ledPath->setText(ui->lwgMissing->currentItem()->text());
    }
    else
    {
        if (ui->lwgAll->currentRow() == -1 || ui->lwgAll->selectedItems().count() > 1) ui->ledPath->clear();
        else ui->ledPath->setText(ui->lwgAll->currentItem()->text());
    }
}

void wdgTab::on_twgItems_currentChanged(int index) { Q_UNUSED(index); setPath(); }
void wdgTab::on_lwgAll_currentRowChanged(int currentRow) { Q_UNUSED(currentRow); setPath(); }
void wdgTab::on_lwgMissing_currentRowChanged(int currentRow) { Q_UNUSED(currentRow); setPath(); }

void wdgTab::on_ledPath_textChanged(const QString &arg1) { ui->btnCopyPath->setEnabled(!arg1.isEmpty()); }

void wdgTab::on_btnCopyPath_clicked()
{
    misc.copy(ui->ledPath->text());
}

