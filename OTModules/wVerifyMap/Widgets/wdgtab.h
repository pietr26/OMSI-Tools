#ifndef WDGTAB_H
#define WDGTAB_H

#include "OTBackend/OTGlobal.h"
#include "OTModules/wContentSearch/wcontentsearch.h"
#include "OTBackend/OTFileSource.h"

#include <QListWidget>
#include <QKeyEvent>

namespace Ui {
class wdgTab;
}

class OTVerificationOverviewData
{
public:
    OTVerificationOverviewData(int _missing, int _existing)
    {
        missing = _missing;
        existing = _existing;
        total = missing + existing;
    }

    int missing;
    int existing;
    int total;
};

class wdgTab : public QWidget
{
    Q_OBJECT

public:
    explicit wdgTab(QWidget *parent = nullptr);
    ~wdgTab();

    void clear();
    void addAll(QList<OTFileSource> items);
    void addMissing(QStringList items);
    void apply();
    void setName(QString name);

    OTVerificationOverviewData getData();

private slots:
    void on_btnSearchForMissingElements_clicked();

    void on_btnCopySelectedElements_clicked();

    void on_twgItems_currentChanged(int index);

    void on_btnCopyPath_clicked();

    void on_btnShowUsages_clicked();

    void on_btnPreviewFile_clicked();

    void on_lwgAll_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgInvalid_itemDoubleClicked(QListWidgetItem *item);

    void on_lwgAll_itemSelectionChanged();

    void on_lwgInvalid_itemSelectionChanged();

    void on_lwgMissing_itemSelectionChanged();

private:
    Ui::wdgTab *ui;
    OTMiscellaneous misc;
    wContentSearch* WCONTENTSEARCH;

    QStringList all, invalid, missing;

    bool isApplied = false;

    void copy(QList<QListWidgetItem*> items);
    void copy(QStringList items);
    void search(QList<QListWidgetItem*> items);
    void search(QStringList items);

    QList<OTFileSource> sources;

    void updateDetails();
    OTFileSource findSource(QString fileName) const;

    QWidget *invalidItesmWidget;
};


class EventFilterCopyElements : public QObject {
    Q_OBJECT

public:
    EventFilterCopyElements(QListWidget *listWidget) : lwg(listWidget) { }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_C)
            {
                QList<QListWidgetItem*> items = lwg->selectedItems();
                QString copystring;

                foreach (QListWidgetItem *current, items) copystring += current->text() + "\n";
                misc.copy(copystring);

                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QListWidget *lwg;
    OTMiscellaneous misc;
};

#endif // WDGTAB_H
