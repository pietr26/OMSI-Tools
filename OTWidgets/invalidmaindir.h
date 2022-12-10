#ifndef INVALIDMAINDIR_H
#define INVALIDMAINDIR_H

#include <QWidget>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class invalidMainDir;
}

class invalidMainDir : public QWidget
{
    Q_OBJECT

public:
    explicit invalidMainDir(QWidget *parent = nullptr);
    ~invalidMainDir();

signals:
    void goToStartScreen();

private slots:
    void on_btnSelectNewPath_clicked();

    void on_btnIgnore_clicked();

private:
    Ui::invalidMainDir *ui;
    OTSettings set;
    OTMiscellaneous misc;
};

#endif // INVALIDMAINDIR_H
