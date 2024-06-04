#ifndef WDGINFORMATION_H
#define WDGINFORMATION_H

#include <QWidget>
#include <QTimer>
#include "OTBackend/LFClientAPIInterface/lfclientapiinterface.h"

namespace Ui {
class wdgInformation;
}

class wdgInformation : public QWidget
{
    Q_OBJECT

public:
    explicit wdgInformation(QWidget *parent, LFClientAPIInterface *api);
    ~wdgInformation();

private:
    Ui::wdgInformation *ui;

    QTimer *timer1s;
    QTimer *timer10s;

    LFClientAPIInterface *api;
    int timeDiff = 0;

private slots:
    void reloadUi1s();
    void reloadUi10s();
};

#endif // WDGINFORMATION_H
