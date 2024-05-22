#ifndef WDGINFORMATION_H
#define WDGINFORMATION_H

#include "OTBackend/OTGlobal.h"
#include <QWidget>

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
    QTimer *timer5s;

    void reloadUi1s();
    void reloadUi5s();

    LFClientAPIInterface *api;
    int timeDiff = 0;
};

#endif // WDGINFORMATION_H
