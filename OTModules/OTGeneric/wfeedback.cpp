#include "wfeedback.h"
#include "ui_wfeedback.h"

#include <QDesktopServices>

#include "OTBackend/OTInformation.h"
#include "OTBackend/OTLinks.h"

wFeedback::wFeedback(QWidget *parent, QUrl wikiEntry) :
    QMainWindow(parent),
    ui(new Ui::wFeedback)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);

    // The dialog frees itself when it is closed. Callers keep their pointer in a
    // QPointer, which clears itself at the same moment - the pointer used to be left
    // dangling and the dialog leaked with every single open.
    setAttribute(Qt::WA_DeleteOnClose);
    adjustSize();
    qDebug() << "UI set";

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    // Load prefs
    setWindowTitle(tr("Feedback") + " - " + OTInformation::name);

    if (wikiEntry.isEmpty())
        ui->btnWikiEntry->setEnabled(false);

    wikiurl = wikiEntry;

    qInfo().noquote() << objectName() + " started";
}

wFeedback::~wFeedback()
{
    delete ui;
}

void wFeedback::on_btnOpenSupportThread_clicked()
{
    QDesktopServices::openUrl(OTLinks::support);
    close();
}

void wFeedback::on_btnOpenSurvey_clicked()
{
    QDesktopServices::openUrl(OTLinks::feedbackForm);
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

