#ifndef WMESSAGEVIEWER_H
#define WMESSAGEVIEWER_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wMessageViewer;
}

class wMessageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit wMessageViewer(OTInAppMessage messageData, QWidget *parent = nullptr);
    ~wMessageViewer();

private:
    Ui::wMessageViewer *ui;

    OTSettings set;
};

#endif // WMESSAGEVIEWER_H
