#ifndef WVERIFYMAP_H
#define WVERIFYMAP_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"
#include "OTBackend/OTOmsiFileHandler.h"
#include "OTWidgets/verifymaptools.h"
#include "OTModules/OTGeneric/wpreferences.h"
#include "OTModules/OTGeneric/wfeedback.h"
#include "OTBackend/OCC/OCFS.h"
#include <QListWidgetItem>
#include <QKeySequence>
#include <QKeyEvent>

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

namespace Ui {
class wVerifyMap;
}

class wVerifyMap : public QMainWindow
{
    Q_OBJECT

public:
    explicit wVerifyMap(QWidget *parent = nullptr);
    ~wVerifyMap();

private slots:

    void on_actionClose_triggered();

    void on_btnStartVerifying_clicked();

    void on_actionPreferences_triggered();

    void on_actionStartVerifying_triggered();

    void reloadProgress();

    void on_actionSendFeedback_triggered();

    void on_cobxMapName_currentIndexChanged(int index);

    void on_btnReloadMaps_clicked();

    void on_actionBackToHome_triggered();

    void on_btnVerificationPreferences_clicked();

signals:
    void backToHome();

private:
    Ui::wVerifyMap *ui;
    OTMessage msg;
    OTSettings set;
    wContentSearch *WCONTENTSEARCH;
    wPreferences *WPREFERENCES;

    int cutCount = set.read("main", "mainDir").toString().size() + 1;

    void selectAllAndClear();

    OTOMSIFileHandler filehandler;

    QTimer *watchProgress = new QTimer();

    void startEndWatchProgress(bool state);

    void endVerifying();

    void loadMapList();

    QList<QPair<QString, QString>> mapList;

    bool mapListSetupFinished = false;
    void enableView(bool enable);

};

#endif // WVERIFYMAP_H
