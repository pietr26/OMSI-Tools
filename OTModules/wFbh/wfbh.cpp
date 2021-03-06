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
    resize(misc.sizeWindow(0.4, 0.8));
    move(misc.centerPosition(this));
    qDebug() << "UI set";

    // Load settings
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

    setWindowTitle("Filebase-Helper (" + OTVersion + ")");

    ui->actionSettings->setText(tr("Settings (%1)").arg(OTName));

    finishedSetup = true;

#ifdef QT_DEBUG
    ui->btnInputExampleReasons->setVisible(true);
    ui->btnFSelectAll->setVisible(true);
#else
    ui->btnInputExampleReasons->setVisible(false);
    ui->btnFSelectAll->setVisible(false);
#endif

    ui->pteLinks->setFocus();

    qInfo().noquote() << objectName() + " started";
}

wFbh::~wFbh()
{
    delete ui;
}

void wFbh::on_actionClose_triggered()
{
    QApplication::quit();
}

void wFbh::on_ledName_textChanged(const QString &arg1)
{
    if (finishedSetup)
        set.write(objectName(), "name", arg1);
}

void wFbh::getLanguage()
{
    if (ui->rbtnGerman->isChecked())
        language = de;
    else
        language = en;
}

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

void wFbh::setFooterSelection(int selectValue)
{
    if (selectedFooter >= selectValue)
        return;
    else
        selectedFooter = selectValue;
}

void wFbh::on_btnAdd_clicked()
{
    ui->gbxAddElement->setVisible(true);
    ui->ledPath->setFocus();
}

void wFbh::on_btnRemove_clicked()
{
    qDeleteAll(ui->lwgCopyrightItems->selectedItems());
    ui->lwgCopyrightItems->setCurrentRow(ui->lwgCopyrightItems->count() - 1);
}

void wFbh::on_btnRemoveAll_clicked()
{
    ui->lwgCopyrightItems->selectAll();
    qDeleteAll(ui->lwgCopyrightItems->selectedItems());
}

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

void wFbh::on_btnInputExampleReasons_clicked()
{
    ui->lwgCopyrightItems->addItem(QString("[tt]Beispielpfad %1[/tt]: Beispielgrund %1").arg(QString::number(1 + ui->lwgCopyrightItems->count())));

    ui->lwgCopyrightItems->clearSelection();
    ui->lwgCopyrightItems->setCurrentRow(ui->lwgCopyrightItems->count() - 1);
}

void wFbh::on_lwgCopyrightItems_currentRowChanged(int currentRow)
{
    Q_UNUSED(currentRow);
    ui->lblReasonCount->setText("Anzahl: " + QString::number(ui->lwgCopyrightItems->count()));

    if (ui->lwgCopyrightItems->count() == 0)
        ui->lblReasonCount->setText("Anzahl: 0");
}

void wFbh::on_btnFSelectAll_clicked()
{
    setFVisible(true);
}

void wFbh::on_cbxFTrademark_stateChanged(int arg1)
{
    if (arg1 == 2)
        ui->cobxFTrademark->setEnabled(true);
    else
        ui->cobxFTrademark->setEnabled(false);
}

void wFbh::on_cbxFCopyright_stateChanged(int arg1)
{
    if (arg1 == 2)
        ui->gbxCopyright->setVisible(true);
    else
        ui->gbxCopyright->setVisible(false);
}

void wFbh::on_cobxWelcome_currentIndexChanged(int index)
{
    if (finishedSetup)
        set.write(objectName(), "welcome", index);
}

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
            copytext += r.FehlendeBeschreibung.de;
        else
            copytext += r.FehlendeBeschreibung.en;

        setFooterSelection(1);
    }

    if (ui->cbxFPictures->isChecked())
    {
        if (language == de)
            copytext += r.FehlendeBilder.de;
        else
            copytext += r.FehlendeBilder.en;

        setFooterSelection(1);
    }

    if (ui->cbxFEnglishDescription->isChecked())
    {
        if (language == de)
            copytext += r.EnBeschreibung.de;
        else
            copytext += r.EnBeschreibung.en;

        setFooterSelection(1);
    }

    if (ui->cbxFExternal->isChecked())
    {
        if (language == de)
            copytext += r.ExternerDownload.de;
        else
            copytext += r.ExternerDownload.en;

        setFooterSelection(4);
    }

    if (ui->cbxFFolderStructure->isChecked())
    {
        if (language == de)
            copytext += r.Ordnerstruktur.de;
        else
            copytext += r.Ordnerstruktur.en;

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
            copytext += r.Projektvorstellung.de;
        else
            copytext += r.Projektvorstellung.en;

        setFooterSelection(4);
    }

    if (ui->cbxFCopyright->isChecked())
    {
        if (language == de)
            copytext += r.Urheberrechtsverletzung.deStart;
        else
            copytext += r.Urheberrechtsverletzung.enStart;

        for (int i = 0; i < ui->lwgCopyrightItems->count(); i++)
            copytext += "<li>" + ui->lwgCopyrightItems->item(i)->text() + "</li>";

        if (language == de)
            copytext += r.Urheberrechtsverletzung.deEnd;
        else
            copytext += r.Urheberrechtsverletzung.enEnd;

        setFooterSelection(1);
    }

    if (ui->cbxFPreviewPicture->isChecked())
    {
        if (language == de)
            copytext += r.FehlendesVorschaubild.de;
        else
            copytext += r.FehlendesVorschaubild.en;

        setFooterSelection(1);
    }

    if (ui->cbxFInstruction->isChecked())
    {
        if (language == de)
            copytext += r.Einbauanleitung.de;
        else
            copytext += r.Einbauanleitung.en;

        setFooterSelection(3);
    }

    if (ui->cbxFArchiveDamaged->isChecked())
    {
        if (language == de)
            copytext += r.ArchivDefekt.de;
        else
            copytext += r.ArchivDefekt.en;

        setFooterSelection(1);
    }

    if (ui->cbxFNoRef->isChecked())
    {
        if (language == de)
            copytext += r.KeinBezug.de;
        else
            copytext += r.KeinBezug.en;

        setFooterSelection(3);
    }

    if (ui->cbxFNoValue->isChecked())
    {
        if (language == de)
            copytext += r.KeinMehrwert.de;
        else
            copytext += r.KeinMehrwert.en;

        setFooterSelection(4);
    }

    if (ui->cbxFTrademark->isChecked())
    {
        if (language == de)
            copytext += r.Markenrecht.de;
        else
            copytext += r.Markenrecht.en;

        setFooterSelection(2);
    }

    if (ui->cbxFContactDetails->isChecked())
    {
        if (language == de)
            copytext += r.Kontaktdaten.de;
        else
            copytext += r.Kontaktdaten.en;

        setFooterSelection(0);
    }

    copytext += "</ul>";

    if (ui->cbxFAdmonition->isChecked())
    {
        if (language == de)
            copytext += r.Verwarnung.de;
        else
            copytext += r.Verwarnung.en;
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

void wFbh::on_btnFClear_clicked()
{
    setFVisible(false);
}

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

        // Androhung der Verwarnung:
        if(ui->cbxRAdmonition->isChecked())
            copytext += r.Verwarnung.de;

        copytext += "<p><br></p><p>Viele Gr????e";
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

        // Androhung der Verwarnung:
        if (ui->cbxRAdmonition->isChecked())
            copytext += r.Verwarnung.en;

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

void wFbh::on_btnRClear_clicked()
{
    ui->cbxRAdmonition->setChecked(false);
}

/// Opens settings (MainApp)
void wFbh::on_actionSettings_triggered()
{
    WSETTINGS = new wSettings(this);
    WSETTINGS->setWindowModality(Qt::ApplicationModal);
    WSETTINGS->show();
}

