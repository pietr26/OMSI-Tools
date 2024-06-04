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
    explicit wdgInformation(QWidget *parent = nullptr, LFClientAPIInterface *api = nullptr);
    ~wdgInformation();

private:
    Ui::wdgInformation *ui;

    QTimer *timer1s;
    QTimer *timer10s;

    void reloadUi1s();
    void reloadUi10s();

    LFClientAPIInterface *api;
    int timeDiff = 0;
};

#endif // WDGINFORMATION_H
