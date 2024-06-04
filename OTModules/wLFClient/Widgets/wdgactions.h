#ifndef WDGACTIONS_H
#define WDGACTIONS_H

#include <QWidget>

#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"

namespace Ui {
class wdgActions;
}

class wdgActions : public QWidget
{
    Q_OBJECT

public:
    explicit wdgActions(QWidget *parent, LFClientAPIInterface *api);
    ~wdgActions();

private slots:
    void on_btnSpeak_clicked();

    void on_btnCrash_clicked();

private:
    Ui::wdgActions *ui;

    void updateButtons();
};

#endif // WDGACTIONS_H
