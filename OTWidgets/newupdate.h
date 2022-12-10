#ifndef NEWUPDATE_H
#define NEWUPDATE_H

#include <QWidget>
#include "OTModules/OTGeneric/wreleasenotes.h"

namespace Ui {
class newUpdate;
}

class newUpdate : public QWidget
{
    Q_OBJECT

public:
    explicit newUpdate(QStringList updateInformation, QWidget *parent = nullptr);
    ~newUpdate();

signals:
    void goToStartScreen();

private slots:
    void on_btnView_clicked();

    void on_btnDisard_clicked();

private:
    Ui::newUpdate *ui;

    wReleaseNotes *WRELEASENOTES;
    QStringList updateInformation;
};

#endif // NEWUPDATE_H
