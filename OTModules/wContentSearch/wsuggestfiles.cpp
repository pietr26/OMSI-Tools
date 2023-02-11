#include "wsuggestfiles.h"
#include "ui_wsuggestfiles.h"

wSuggestFiles::wSuggestFiles(QWidget *parent, QString links) :
    QMainWindow(parent),
    ui(new Ui::wSuggestFiles)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    //adjustSize();
    resize(misc.sizeWindow(0.4, 0.65));
    qDebug() << "UI set";

    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    if (!links.isEmpty())
        isReport = true;

    // Load settings
    setStyleSheet(set.read("main", "theme").toString());

    nc.post(OTLinks::empty);

    if (nc.lastSuccess == -2)
    {
        QMessageBox::information(this, tr("Maintenance"), OTStrings::serverMaintenance());
        QTimer::singleShot(1, this, &wSuggestFiles::close);
        return;
    }
    else if (nc.lastSuccess == -1)
    {
        QMessageBox::information(this, tr("No server connection"), tr("Could not connect to application server. Try again later."));
        QTimer::singleShot(1, this, &wSuggestFiles::close);
        return;
    }

    if (isReport)
    {
        setWindowTitle(tr("Report invalid link(s)"));
        ui->pteReport->setPlainText(links);
        ui->pteFiles->setPlainText("placeholder");
    }
    else
    {
        setWindowTitle(tr("Suggest files for content search"));
        on_pteFiles_textChanged();
    }

    ui->lblFiles->setVisible(!isReport);
    ui->pteFiles->setVisible(!isReport);
    ui->lblReport->setVisible(isReport);
    ui->pteReport->setVisible(isReport);

    on_btnRules_clicked();

    qInfo().noquote() << objectName() + " started";
}

wSuggestFiles::~wSuggestFiles()
{
    delete ui;
}

void wSuggestFiles::on_btnRules_clicked()
{
    if (isReport)
        QMessageBox::information(this, tr("Rules for reporting invalid links"), tr("In order to report invalid links, the following rule must be accepted:\n\nDo not propose the same links more than once and avoid spam."));
    else
        QMessageBox::information(this, tr("Rules for suggesting files"), tr("In order to propose files, the following rules must be accepted:\n\n1) The suggested files must be publicly available.\n2) In general, the file should be copyright correct\n3) Do not suggest files from the website omsi-webdisk.de. These will be automatically added to the database periodically.\n4) Do not propose the same files more than once and avoid spam. Make only one submission even for several files."));
}

void wSuggestFiles::on_pteFiles_textChanged()
{
    if (!ui->pteFiles->toPlainText().isEmpty() && ui->cbxAcceptRules->isChecked())
        ui->btnSend->setEnabled(true);
    else
        ui->btnSend->setEnabled(false);
}

void wSuggestFiles::on_btnSend_clicked()
{
    if (isReport)
    {
        qInfo() << nc.post(OTLinks::csReportLink,
                QList<QPair<QString, QString>> {
                    QPair<QString, QString>("link", ui->pteReport->toPlainText()),
                    QPair<QString, QString>("notes", ui->pteNotes->toPlainText()),
                });

        if (nc.lastSuccess == 1)
        {
            QMessageBox::information(this, tr("Suggestion sent"), tr("The suggestion has been sent. Thank you!"));
            close();
        }
        else
            QMessageBox::critical(this, tr("Cannot send suggestion"), tr("The suggestion could not be sent. Please try again later."));
    }
    else
    {
        qInfo() << nc.post(OTLinks::csSuggestFiles,
                QList<QPair<QString, QString>> {
                    QPair<QString, QString>("files", ui->pteFiles->toPlainText()),
                    QPair<QString, QString>("notes", ui->pteNotes->toPlainText()),
                });

        if (nc.lastSuccess == 1)
        {
            QMessageBox::information(this, tr("Suggestion sent"), tr("The suggestion has been sent. Thank you!"));
            close();
        }
        else
            QMessageBox::critical(this, tr("Cannot send suggestion"), tr("The suggestion could not be sent. Please try again later."));
    }
}

void wSuggestFiles::on_cbxAcceptRules_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    on_pteFiles_textChanged();
}

