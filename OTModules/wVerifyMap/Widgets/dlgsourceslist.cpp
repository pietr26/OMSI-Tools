#include "dlgsourceslist.h"
#include "ui_dlgsourceslist.h"

#include <QClipboard>

dlgSourcesList::dlgSourcesList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dlgSourcesList)
{
    ui->setupUi(this);

    ui->twgSources->setColumnHidden(0, true);

    ui->twgSources->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->twgSources->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->twgSources->setColumnWidth(0, 0);
    ui->twgSources->setColumnWidth(2, 50);

    ui->twgSources->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *action = ui->twgSources->addAction(QIcon::fromTheme(QIcon::ThemeIcon::EditCopy), tr("Copy"));
    action->setShortcut(QKeySequence::Copy);

    connect(action, &QAction::triggered, this, &dlgSourcesList::onCopySelected);
    connect(ui->btnCopySelected, &QPushButton::clicked, this, &dlgSourcesList::onCopySelected);
}

dlgSourcesList::~dlgSourcesList()
{
    delete ui;
}

void dlgSourcesList::addSources(const QList<QPair<QString, int> > &list) {
    for(QPair<QString, int> current : list) {
        QTreeWidgetItem *itm = new QTreeWidgetItem;
        itm->setText(1, current.first);
        itm->setText(2, QString::number(current.second));
        ui->twgSources->addTopLevelItem(itm);
    }
}

void dlgSourcesList::onCopySelected() {
    QStringList strings;
    QList<QTreeWidgetItem *> items = ui->twgSources->selectedItems();
    for(QTreeWidgetItem *itm : items)
        strings << itm->text(1);

    copyListToClipboard(strings);
}

void dlgSourcesList::on_btnCopyAll_clicked() {
    QStringList strings;
    for(int i = 0; i < ui->twgSources->topLevelItemCount(); i++)
        strings << ui->twgSources->topLevelItem(i)->text(1);

    copyListToClipboard(strings);
}

void dlgSourcesList::copyListToClipboard(const QStringList &list) {
    QClipboard *clipboard = qApp->clipboard();
    clipboard->setText(list.join("\r\n"));
}
