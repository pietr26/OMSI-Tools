#include "wdgtab.h"
#include "ui_wdgtab.h"

#include "dlgsourceslist.h"
#include "dlgfilepreview.h"

wdgTab::wdgTab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::wdgTab)
{
    ui->setupUi(this);
    ui->lwgAll->installEventFilter(new EventFilterCopyElements(ui->lwgAll));
    ui->lwgMissing->installEventFilter(new EventFilterCopyElements(ui->lwgMissing));

    // save the tab widget of "invalid" because we need to remove it if advanced mode is not enabled but we need to add it later again
    invalidItesmWidget = ui->twgItems->widget(1);
    clear();
}

wdgTab::~wdgTab()
{
    delete ui;
}

void wdgTab::clear()
{
    OTSettings set;
    bool advanced = set.read("wVerifyMap", "advVerifying").toBool();

    all.clear();
    invalid.clear();
    missing.clear();
    sources.clear();

    ui->lwgAll->clear();
    ui->lwgInvalid->clear();
    ui->lwgMissing->clear();

    if(advanced) {
        if(ui->twgItems->count() == 2)
            ui->twgItems->insertTab(1, invalidItesmWidget, tr("Invalid (%1)").arg(0));
        else
            ui->twgItems->setTabText(1, tr("Invalid (%1)").arg(0));

        ui->twgItems->setTabText(0, tr("All (%1)").arg(0));
        ui->twgItems->setTabText(2, tr("Missing (%1)").arg(0));
    } else {
        if(ui->twgItems->count() == 3)
            ui->twgItems->removeTab(1);

        ui->twgItems->setTabText(0, tr("All (%1)").arg(0));
        ui->twgItems->setTabText(1, tr("Missing (%1)").arg(0));
    }

    ui->ledPath->clear();
    ui->lblStatus->clear();
    ui->lblUsages->clear();
}

void wdgTab::addAll(QList<OTFileSource> items)
{
    isApplied = false;
    for(OTFileSource &source : items) {
        all << source.fileName().replace("/", "\\");
        sources << source;
        if(!source.advancedCheckResult().isValid()) {
            invalid<< source.fileName().replace("/", "\\");
        }
    }
}

void wdgTab::addMissing(QStringList items) {
    isApplied = false;
    items.replaceInStrings("/", "\\");
    missing << items;
}

void wdgTab::apply()
{
    ui->lwgMissing->clear();
    ui->lwgMissing->addItems(missing);
    ui->lwgMissing->sortItems();

    ui->lwgInvalid->clear();
    ui->lwgInvalid->addItems(invalid);
    ui->lwgInvalid->sortItems();

    ui->lwgAll->clear();
    ui->lwgAll->addItems(all);
    ui->lwgAll->sortItems();

    for(int i = 0; i < ui->lwgAll->count(); i++)  {
        QListWidgetItem *itm = ui->lwgAll->item(i);
        QString text = itm->text();
        if(missing.contains(text)) {
            itm->setBackground(QColor(255, 0, 0, 64));
            itm->setToolTip(tr("This file was not found"));
        } else if(!findSource(text).advancedCheckResult().isValid()) {
            itm->setBackground(QColor(255, 170, 0, 128));
            itm->setToolTip(tr("This file is invalid"));
        }
    }

    isApplied = true;

    OTSettings set;
    bool advanced = set.read("wVerifyMap", "advVerifying").toBool();

    if(advanced) {
        ui->twgItems->setTabText(0, tr("All (%1)").arg(all.count()));
        ui->twgItems->setTabText(1, tr("Invalid (%1)").arg(invalid.count()));
        ui->twgItems->setTabText(2, tr("Missing (%1)").arg(missing.count()));
    } else {
        ui->twgItems->setTabText(0, tr("All (%1)").arg(all.count()));
        ui->twgItems->setTabText(1, tr("Missing (%1)").arg(missing.count()));
    }
}

void wdgTab::setName(QString name)
{
    ui->btnCopySelectedElements->setText(tr("Copy selected %1", "Copy selected elements").arg(name));
    ui->btnSearchForMissingElements->setText(tr("Search for missing %1", "Search for missing elements").arg(name));
}

OTVerificationOverviewData wdgTab::getData()
{
    if (!isApplied) qFatal() << "Cannot get data from an unapplied object!";
    return OTVerificationOverviewData(missing.count(), all.count());
}

void wdgTab::on_btnSearchForMissingElements_clicked()
{
    if (ui->lwgMissing->selectedItems().isEmpty()) search(missing);
    else search(ui->lwgMissing->selectedItems());
}

void wdgTab::on_btnCopySelectedElements_clicked()
{
    if (ui->lwgMissing->selectedItems().isEmpty()) copy(ui->twgItems->currentIndex() ? missing : all);
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

void wdgTab::updateDetails()
{
    int tabIndex = ui->twgItems->currentIndex();
    QListWidgetItem *itm;
    int selectionCount;

    if(tabIndex == 0) {
        itm = ui->lwgAll->currentItem();
        selectionCount = ui->lwgAll->selectedItems().count();
    } else if(tabIndex == 1) {
        itm = ui->lwgInvalid->currentItem();
        selectionCount = ui->lwgInvalid->selectedItems().count();
    } else {
        itm = ui->lwgMissing->currentItem();
        selectionCount = ui->lwgMissing->selectedItems().count();
    }

    // clear if invalid
    if(!itm || selectionCount > 1) {
        ui->ledPath->clear();
        ui->lblStatus->clear();
        ui->lblUsages->clear();
        return;
    }

    // else set values
    ui->ledPath->setText(itm->text());
    OTFileSource source = findSource(itm->text());
    ui->lblUsages->setText(tr("%1 (in %n file(s))", "", source.sourcesCount()).arg(QString::number(source.occurrencesCount())));

    bool isMissing = missing.contains(itm->text());

    if(isMissing) {
        ui->lblStatus->setText(tr("missing"));
        ui->lblStatus->setStyleSheet("color: #ff2222;");
    } else {
        if(source.advancedCheckResult().isValid()) {
            ui->lblStatus->setText(tr("valid"));
            ui->lblStatus->setStyleSheet("color: #55aa00;");
        } else {
            ui->lblStatus->setText(tr("%n issue(s) found", "", source.advancedCheckResult().issueCount()));
            ui->lblStatus->setStyleSheet("color: #ffaa00;");
        }
    }
}

OTFileSource wdgTab::findSource(QString fileName) const {
    for(OTFileSource source : sources)
        if(source.fileName() == fileName)
            return source;

    // fallback - should never be needed
    return OTFileSource("", OTFileSource::UnknownFile);
}

void wdgTab::on_twgItems_currentChanged(int index) { Q_UNUSED(index); updateDetails(); }
void wdgTab::on_lwgAll_currentRowChanged(int currentRow) { Q_UNUSED(currentRow); updateDetails(); }
void wdgTab::on_lwgInvalid_currentRowChanged(int currentRow) { Q_UNUSED(currentRow); updateDetails(); }
void wdgTab::on_lwgMissing_currentRowChanged(int currentRow) { Q_UNUSED(currentRow); updateDetails(); }

void wdgTab::on_ledPath_textChanged(const QString &arg1) { ui->btnCopyPath->setEnabled(!arg1.isEmpty()); }

void wdgTab::on_btnCopyPath_clicked()
{
    misc.copy(ui->ledPath->text());
}

void wdgTab::on_btnShowUsages_clicked() {
    OTFileSource source = findSource(ui->ledPath->text());
    dlgSourcesList dlg;
    QList<QPair<QString, int>> sources = source.sources();
    std::sort(sources.begin(), sources.end(), [](QPair<QString, int> a, QPair<QString, int> b){return a.first < b.first;});

    dlg.addSources(sources);
    dlg.exec();
}

void wdgTab::on_btnPreviewFile_clicked() {
    OTSettings set;
    QString file = set.read("main", "mainDir").toString() + "/" + ui->ledPath->text();
    if(!QFile::exists(file))
        return;

    dlgFilePreview dlg(this, file, findSource(ui->ledPath->text()).advancedCheckResult());
    dlg.exec();
}

void wdgTab::on_lwgAll_itemDoubleClicked(QListWidgetItem *item) {
    Q_UNUSED(item);
    on_btnPreviewFile_clicked();
}

void wdgTab::on_lwgInvalid_itemDoubleClicked(QListWidgetItem *item) {
    Q_UNUSED(item);
    on_btnPreviewFile_clicked();
}
