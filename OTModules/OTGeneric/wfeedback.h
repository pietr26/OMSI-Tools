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
    explicit wFeedback(QWidget *parent = nullptr, QUrl wikiEntry = QUrl());
    ~wFeedback();

private slots:
    void on_btnOpenSupportThread_clicked();

    void on_btnOpenSurvey_clicked();

    void on_btnWikiEntry_clicked();

    void on_btnCopyMail_clicked();

private:
    Ui::wFeedback *ui;
    OTSettings set;
    OTMiscellaneous misc;

    QUrl wikiurl;
};

#endif // WFEEDBACK_H
