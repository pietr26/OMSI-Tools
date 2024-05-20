#ifndef WDGINFORMATION_H
#define WDGINFORMATION_H

#include <QWidget>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wdgInformation;
}

class wdgInformation : public QWidget
{
    Q_OBJECT

public:
    explicit wdgInformation(QWidget *parent = nullptr);
    ~wdgInformation();

private:
    Ui::wdgInformation *ui;

    QTimer *timer1s;
    QTimer *timer5s;

    void reloadUi1s();
    void reloadUi5s();

    OTNetworkConnection nc;
};

#endif // WDGINFORMATION_H
