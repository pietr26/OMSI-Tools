#ifndef WFEEDBACK_H
#define WFEEDBACK_H

#include <QMainWindow>
#include "OTBackend/OTGlobal.h"

namespace Ui {
class wFeedback;
}

class wFeedback : public QMainWindow
{
    Q_OBJECT

public:
    explicit wFeedback(QWidget *parent = nullptr);
    ~wFeedback();

private slots:
    void on_btnOpenSupportThread_clicked();

    void on_btnOpenSurvey_clicked();

private:
    Ui::wFeedback *ui;
    const QString moduleName = "wChangelog";
    OTSettings set;
    //OTMiscellaneous misc;
};

#endif // WFEEDBACK_H
