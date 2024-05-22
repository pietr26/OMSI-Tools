#ifndef WDGNOTIFICATION_H
#define WDGNOTIFICATION_H

#include <QWidget>

namespace Ui {
class wdgNotification;
}

class wdgNotification : public QWidget
{
    Q_OBJECT

public:
    explicit wdgNotification(QStringList lines, QString title, QString text, QWidget *parent = nullptr);
    ~wdgNotification();

private:
    Ui::wdgNotification *ui;
};

#endif // WDGNOTIFICATION_H
