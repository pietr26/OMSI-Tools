#include "wfbh.h"
#include "ui_wfbh.h"

#include <QMainWindow>

wFbh::wFbh(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::wFbh)
{
    qInfo().noquote() << "Starting " + objectName() + "...";

    qDebug() << "Set up UI...";
    ui->setupUi(this);
    qDebug() << "UI set";

    // Load prefs
    setStyleSheet(set.read("main", "theme").toString());

    ui->cobxWelcome->addItem("Hallo");
    ui->cobxWelcome->addItem("Moin");
    ui->cobxWelcome->addItem("Servus");
    ui->cobxWelcome->addItem("Guten Tag");
    ui->cobxWelcome->addItem("Hi");
    ui->cobxWelcome->addItem("Hey");
    ui->cobxWelcome->setCurrentIndex(set.read(objectName(), "welcome").toInt());

    ui->ledName->setText(set.read(objectName(), "name").toString());

    ui->gbxCopyright->setVisible(false);
    ui->gbxAddElement->setVisible(false);

    setWindowTitle("Filebase-Helper (" + OTInformation::versions::currentVersion.first + ")");

    ui->actionPreferences->setText(tr("Preferences (%1)").arg(OTInformation::name));

    finishedSetup = true;

#ifdef QT_DEBUG
    ui->btnInputExampleReasons->setVisible(true);
    ui->btnFSelectAll->setVisible(true);
#else
    ui->btnInputExampleReasons->setVisible(false);
    ui->btnFSelectAll->setVisible(false);
#endif

    ui->pteLinks->setFocus();

    qApp->processEvents();
    adjustSize();
    move(misc.centerPosition(this));

    qInfo().noquote() << objectName() + " started";
}

wFbh::~wFbh()
{
    delete ui;
}

/// Closes the window
void wFbh::on_actionClose_triggered()
{
    close();
}

/// Saves new name
void wFbh::on_ledName_textChanged(const QString &arg1)
{
    if (finishedSetup)
        set.write(objectName(), "name", arg1);
}

/// Switches language
void wFbh::getLanguage()
{
    if (ui->rbtnGerman->isChecked())
        language = de;
    else
        language = en;
}

/// URL parser - returns HTML
QString wFbh::urlParser(QString links, bool review)
{
    QString pteContent = ui->pteLinks->toPlainText();
    QTextStream in(&links, QIODevice::ReadOnly);

    QString result;

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (review)
            return line;

        bool isNumber = false;
        int fileID = line.toInt(&isNumber);

        if (isNumber)
            result += QString("<p>https://reboot.omsi-webdisk.de/file/%1-</p>").arg(QString::number(fileID));

        else if (line.endsWith("#overview", Qt::CaseInsensitive) ||
                 line.endsWith("#versions", Qt::CaseInsensitive) ||
                 line.endsWith("#comments", Qt::CaseInsensitive))
            line.chop(9);

        else if (line.endsWith("#overview/", Qt::CaseInsensitive) ||
                 line.endsWith("#versions/", Qt::CaseInsensitive) ||
                 line.endsWith("#comments/", Qt::CaseInsensitive))
            line.chop(10);

        else if (!review)
        {
            if (line.endsWith("#reviews", Qt::CaseInsensitive))
                line.chop(8);

            else if (line.endsWith("#reviews/", Qt::CaseInsensitive))
                line.chop(9);
        }

        if (!isNumber)
            result += QString("<p>%1</p>").arg(line);
    }

    return result;
}

/// (Un)checks all checkboxes
void wFbh::setFVisible(bool visible)
{
    ui->cbxFDescription->setChecked(visible);
    ui->cbxFPictures->setChecked(visible);
    ui->cbxFEnglishDescription->setChecked(visible);
    ui->cbxFExternal->setChecked(visible);
    ui->cbxFFolderStructure->setChecked(visible);
    ui->cbxFScreenshot->setChecked(visible);
    ui->cbxFCopyright->setChecked(visible);
    ui->cbxFPreviewPicture->setChecked(visible);
    ui->cbxFPresentation->setChecked(visible);
    ui->cbxFAdmonition->setChecked(visible);
    ui->cbxFInstruction->setChecked(visible);
    ui->cbxFArchiveDamaged->setChecked(visible);
    ui->cbxFNoRef->setChecked(visible);
    ui->cbxFNoValue->setChecked(visible);
    ui->cbxFTrademark->setChecked(visible);
    ui->cbxFContactDetails->setChecked(visible);
}

/// Selects footer for message
void wFbh::setFooterSelection(int selectValue)
{
    if (selectedFooter >= selectValue)
        return;
    else
        selectedFooter = selectValue;
}

/// Shows line edits for entering a reason for copyright violations
void wFbh::on_btnAdd_clicked()
{
    ui->gbxAddElement->setVisible(true);
    ui->ledPath->setFocus();
}

/// Removes a reason for copyright violations
void wFbh::on_btnRemove_clicked()
{
    qDeleteAll(ui->lwgCopyrightItems->selectedItems());
    ui->lwgCopyrightItems->setCurrentRow(ui->lwgCopyrightItems->count() - 1);
}

/// Remove all reasons for copyright violations
void wFbh::on_btnRemoveAll_clicked()
{
    ui->lwgCopyrightItems->selectAll();
    qDeleteAll(ui->lwgCopyrightItems->selectedItems());
}

/// Adds a reason for copyright violations
void wFbh::on_btnAddElement_clicked()
{
    QString item = "[tt]" + ui->ledPath->text() + "[/tt]: " + ui->ledInfos->text();
    ui->lwgCopyrightItems->addItem(item);

    ui->gbxAddElement->setVisible(false);
    ui->lwgCopyrightItems->setFocus();

    ui->lwgCopyrightItems->clearSelection();
    ui->lwgCopyrightItems->setCurrentRow(ui->lwgCopyrightItems->count() - 1);

    ui->ledPath->clear();
    ui->ledInfos->clear();
}

/// Inserts example reasions (debug)
void wFbh::on_btnInputExampleReasons_clicked()
{
    ui->lwgCopyrightItems->addItem(QString("[tt]Beispielpfad %1[/tt]: Beispielgrund %1").arg(QString::number(1 + ui->lwgCopyrightItems->count())));

    ui->lwgCopyrightItems->clearSelection();
    ui->lwgCopyrightItems->setCurrentRow(ui->lwgCopyrightItems->count() - 1);
}

/// Checks all file checkboxes (debug)
void wFbh::on_btnFSelectAll_clicked()
{
    setFVisible(true);
}

/// Enables / disables file trademark combobox
void wFbh::on_cbxFTrademark_stateChanged(int arg1)
{
    if (arg1 == 2)
        ui->cobxFTrademark->setEnabled(true);
    else
        ui->cobxFTrademark->setEnabled(false);
}

/// Shows / hides file copyright violation section
void wFbh::on_cbxFCopyright_stateChanged(int arg1)
{
    if (arg1 == 2)
        ui->gbxCopyright->setVisible(true);
    else
        ui->gbxCopyright->setVisible(false);

    qApp->processEvents();
    adjustSize();
}

/// Sets welcome text
void wFbh::on_cobxWelcome_currentIndexChanged(int index)
{
    if (finishedSetup)
        set.write(objectName(), "welcome", index);
}

/// Copies file message
void wFbh::on_btnFCopy_clicked()
{
    selectedFooter = 0;
    QString copytext;
    getLanguage();

    QString fileURLs = urlParser(ui->pteLinks->toPlainText());
    // Header:
    if (language == de)
        copytext = "<p>" + ui->cobxWelcome->currentText() + r.Header.de + "<p>" + fileURLs + "</p><p><br></p>" + r.Header2.de;
    else
        copytext = r.Header.en + "<p>" + fileURLs + "</p><p><br></p>" + r.Header2.en;

    if (ui->cbxFDescription->isChecked())
    {
        if (language == de)
            copytext += r.MissingDescription.de;
        else
            copytext += r.MissingDescription.en;

        setFooterSelection(1);
    }

    if (ui->cbxFPictures->isChecked())
    {
        if (language == de)
            copytext += r.MissingPictures.de;
        else
            copytext += r.MissingPictures.en;

        setFooterSelection(1);
    }

    if (ui->cbxFEnglishDescription->isChecked())
    {
        if (language == de)
            copytext += r.EnDescription.de;
        else
            copytext += r.EnDescription.en;

        setFooterSelection(1);
    }

    if (ui->cbxFExternal->isChecked())
    {
        if (language == de)
            copytext += r.ExternalDownload.de;
        else
            copytext += r.ExternalDownload.en;

        setFooterSelection(4);
    }

    if (ui->cbxFFolderStructure->isChecked())
    {
        if (language == de)
            copytext += r.Filestructure.de;
        else
            copytext += r.Filestructure.en;

        setFooterSelection(1);
    }

    if (ui->cbxFScreenshot->isChecked())
    {
        if (language == de)
            copytext += r.Screenshot.de;
        else
            copytext += r.Screenshot.en;

        setFooterSelection(4);
    }

    if (ui->cbxFPresentation->isChecked())
    {
        if (language == de)
            copytext += r.Projectpresentation.de;
        else
            copytext += r.Projectpresentation.en;

        setFooterSelection(4);
    }

    if (ui->cbxFCopyright->isChecked())
    {
        if (language == de)
            copytext += r.Copyrightviolation.deStart;
        else
            copytext += r.Copyrightviolation.enStart;

        for (int i = 0; i < ui->lwgCopyrightItems->count(); i++)
            copytext += "<li>" + ui->lwgCopyrightItems->item(i)->text() + "</li>";

        if (language == de)
            copytext += r.Copyrightviolation.deEnd;
        else
            copytext += r.Copyrightviolation.enEnd;

        setFooterSelection(1);
    }

    if (ui->cbxFPreviewPicture->isChecked())
    {
        if (language == de)
            copytext += r.MissingPreviewPicture.de;
        else
            copytext += r.MissingPreviewPicture.en;

        setFooterSelection(1);
    }

    if (ui->cbxFInstruction->isChecked())
    {
        if (language == de)
            copytext += r.Installationinstruction.de;
        else
            copytext += r.Installationinstruction.en;

        setFooterSelection(3);
    }

    if (ui->cbxFArchiveDamaged->isChecked())
    {
        if (language == de)
            copytext += r.ArchiveDamaged.de;
        else
            copytext += r.ArchiveDamaged.en;

        setFooterSelection(1);
    }

    if (ui->cbxFNoRef->isChecked())
    {
        if (language == de)
            copytext += r.NoReference.de;
        else
            copytext += r.NoReference.en;

        setFooterSelection(3);
    }

    if (ui->cbxFNoValue->isChecked())
    {
        if (language == de)
            copytext += r.NoValue.de;
        else
            copytext += r.NoValue.en;

        setFooterSelection(4);
    }

    if (ui->cbxFTrademark->isChecked())
    {
        if (language == de)
            copytext += r.Trademarkviolation.de;
        else
            copytext += r.Trademarkviolation.en;

        setFooterSelection(2);
    }

    if (ui->cbxFContactDetails->isChecked())
    {
        if (language == de)
            copytext += r.Contactdetails.de;
        else
            copytext += r.Contactdetails.en;

        setFooterSelection(0);
    }

    copytext += "</ul>";

    if (ui->cbxFAdmonition->isChecked())
    {
        if (language == de)
            copytext += r.Admonition.de;
        else
            copytext += r.Admonition.en;
    }

    // Footer:
    if (language == de)
        copytext += r.getFooter(selectedFooter, de);
    else
        copytext += r.getFooter(selectedFooter, en);

    if (ui->ledName->text() != "")
    {
        copytext += ",</p><p>";
        copytext += ui->ledName->text();
        copytext += "</p>";
    }
    else
        copytext += "</p>";

    misc.copy(copytext);
}

/// Unnchecks all file checkboxes
void wFbh::on_btnFClear_clicked()
{
    setFVisible(false);
}

/// Copies review message
void wFbh::on_btnRCopy_clicked()
{
    QString copytext;
    getLanguage();

    if (language == de)
    {
        copytext = "<p>" + ui->cobxWelcome->currentText() + r.rHeader.de + "<p>" + urlParser(ui->pteLinks->toPlainText()) + "</p><p><br></p>";
        copytext += "<woltlab-quote data-author=\"";
        copytext += ui->ledRTitle->text();
        copytext += "\" data-link=\"";
        copytext += urlParser(ui->pteLinks->toPlainText(), true);
        copytext += "\">";
        copytext += ui->pteRDescription->toPlainText();
        copytext += "</woltlab-quote>";
        copytext += r.rMiddle.de;
        if (ui->cbxRRules->isChecked() == true)
            copytext += r.rRegeln.de;

        // Androhung der Admonition:
        if(ui->cbxRAdmonition->isChecked())
            copytext += r.Admonition.de;

        copytext += "<p><br></p><p>Viele Grüße";
        if (ui->ledName->text() != "")
        {
            copytext += ",</p><p>";
            copytext += ui->ledName->text();
            copytext += "</p>";
        }
        else
        copytext += "</p>";
    }
    else
    {
        copytext = r.rHeader.en + "<p>" + urlParser(ui->pteLinks->toPlainText()) + "</p><p><br></p>";
        copytext += "<woltlab-quote data-author=\"";
        copytext += ui->ledRTitle->text();
        copytext += "\" data-link=\"";
        copytext += urlParser(ui->pteLinks->toPlainText());
        copytext += "\">";
        copytext += ui->pteRDescription->toPlainText();
        copytext += "</woltlab-quote>";
        copytext += r.rMiddle.en;
        if (ui->cbxRRules->isChecked() == true)
            copytext += r.rRegeln.en;

        // Androhung der Admonition:
        if (ui->cbxRAdmonition->isChecked())
            copytext += r.Admonition.en;

        copytext += "<p><br></p><p>Best regards";
        if (ui->ledName->text() != "")
        {
            copytext += ",</p><p>";
            copytext += ui->ledName->text();
            copytext += "</p>";
        }
        else
            copytext += "</p>";
    }

    misc.copy(copytext);
}

/// Unchecks all review checkboxes
void wFbh::on_btnRClear_clicked()
{
    ui->cbxRAdmonition->setChecked(false);
}

/// Opens prefs (mainApp)
void wFbh::on_actionPreferences_triggered()
{
    WPREFERENCES = new wPreferences();
    WPREFERENCES->setWindowModality(Qt::ApplicationModal);
    WPREFERENCES->show();
}

