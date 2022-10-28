#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>
#include "OTBackend/OTGlobal.h"
#include "OTModules/wmessageviewer.h"

namespace Ui {
class message;
}

class message : public QWidget
{
    Q_OBJECT

public:
    explicit message(OTInAppMessage paramMessageData, QWidget *parent = nullptr);
    ~message();

    void showDescription();

private:
    Ui::message *ui;

    OTInAppMessage messageData;

    OTSettings set;

    void markAsRead();

    wMessageViewer* WMESSAGEVIEWER;
};

#endif // MESSAGE_H
