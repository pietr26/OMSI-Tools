#ifndef WDGNOTIFICATIONS_H
#define WDGNOTIFICATIONS_H

#include <QWidget>

namespace Ui {
class wdgNotifications;
}

class wdgNotifications : public QWidget
{
    Q_OBJECT

public:
    explicit wdgNotifications(QWidget *parent = nullptr);
    ~wdgNotifications();

private:
    Ui::wdgNotifications *ui;
};

#endif // WDGNOTIFICATIONS_H
