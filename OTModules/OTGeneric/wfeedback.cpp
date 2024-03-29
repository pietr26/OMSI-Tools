#include "wfeedback.h"
#include "ui_wfeedback.h"

wFeedback::wFeedback(QWidget *parent, QUrl wikiEntry) :
    QMainWindow(parent),
    ui(new Ui::wFeedback)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    adjustSize();
    qDebug() << "UI set";

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    // Load prefs
    setStyleSheet(set.read("main","theme").toString());
    setWindowTitle(OTInformation::name + " - " + tr("Feedback"));

    ui->btnOpenSurvey->setVisible(false);

    if (wikiEntry.isEmpty())
        ui->btnWikiEntry->setEnabled(false);

    wikiurl = wikiEntry;

    qInfo().noquote() << objectName() + " started";
}

wFeedback::~wFeedback()
{
    delete ui;
}

/// Opens support thread
void wFeedback::on_btnOpenSupportThread_clicked()
{
    QDesktopServices::openUrl(OTLinks::support);
    close();
}

/// Opens survey
void wFeedback::on_btnOpenSurvey_clicked()
{
    QDesktopServices::openUrl(OTLinks::survey);
    close();
}

void wFeedback::on_btnWikiEntry_clicked()
{
    QDesktopServices::openUrl(wikiurl);
    close();
}


void wFeedback::on_btnCopyMail_clicked()
{
    misc.copy(ui->ledMail->text());
}

