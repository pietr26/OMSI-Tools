#ifndef FBHFUNCTIONS_H
#define FBHFUNCTIONS_H

#include <QString>

class cAdmonition
{
public:
    const QString de = "<p>Bitte beachte, dass es bei wiederholten Verstößen zu Ermahnungen oder Verwarnungen kommen kann.</p>";
    const QString en = "<p>Please note that repeated violations may result in admonitions or warnings.</p>";
};

class cHeader
{
public:
    const QString de = ",</p><p>ich kontaktiere dich bezüglich folgender Datei(en):</p>";
    const QString en = "Hello,</p><p>I am contacting you regarding the following file(s):</p>";
};

class cHeader2
{
public:
    const QString de = "<p>Leider konnten wir sie nicht freischalten, da diese nicht unseren Uploadbedingungen entsprechen.</p><ul>";
    const QString en = "<p>Unfortunately, we could not unlock them because they do not comply with our upload conditions.</p><ul>";
};

class cFilestructure
{
public:
    const QString de = "<li><strong>Ordnerstruktur:</strong> Alle Archive müssen den vollständigen OMSI-Pfad enthalten. Die erste Ebene muss entweder mit [tt]\\OMSI 2\\Vehicles[/tt] (o.ä.) oder direkt mit den Unterordnern [tt]\\Vehicles[/tt] (o.ä.) beginnen. Mit dem kostenlosen Programm \"Omsipfad\" lässt sich auf einfache Weise ein Archiv mit korrekter Ordnerstruktur erstellen: <a href=\"https://reboot.omsi-webdisk.de/file/5089-omsipfad-omsipath/\"><strong>Download</strong></a><strong></strong> | <a href=\"https://reboot.omsi-webdisk.de/wiki/entry/74-omsipfad-OrdnerstrukturOrdnerstruktur-fuer-den-upload-erstellen/\"><strong>Erklärung</strong></a></li>";
    const QString en = "<li><strong>Folder structure:</strong> All archives must contain the complete OMSI path. The first level must start either with [tt]\\OMSI 2\\Vehicles[/tt] (or similar) or directly with the subfolders [tt]\\Vehicles[/tt] (or similar). With the free program \"Omsipath\" you can easily create an archive with a correct folder structure: <a href=\"https://reboot.omsi-webdisk.de/file/5089-omsipfad-omsipath/\"><strong>Download</strong></a><strong></strong> | <a href=\"https://reboot.omsi-webdisk.de/wiki/entry/76-omsipfad-create-folder-structure-for-the-upload/\"><strong>Explanation</strong></a></li>";
};

class cMissingPictures
{
public:
    const QString de = "<li><strong>Bilder:</strong> Es fehlen Bilder. Bei allen Datei-Einträgen, bei denen es etwas zu sehen gibt, müssen diese vorhanden sein. Lade diese über die Dateianhangs-Funktion bei der Bearbeitung deines Datei-Eintrags hoch.<br><a href=\"https://reboot.omsi-webdisk.de/wiki/entry/64-hochladen-von-dateien-in-der-filebase/#3.5.1-Bilder-einbinden\"><strong>Infos zu Bildern</strong></a></li>";
    const QString en = "<li><strong>Pictures:</strong> Pictures are missing. All file entries with something to see require pictures. Upload them using the file attachments tab when editing your file entry.<br><a href=\"https://reboot.omsi-webdisk.de/wiki/entry/85-uploading-files-to-the-filebase/#3.5.1-Integrating-pictures\"><strong>Info about pictures</strong></a></li>";
};

class cEnDescription
{
public:
    const QString de = "<li><strong>Englische Beschreibung:</strong> Es fehlt eine englische Beschreibung. Bei allen Dateien außerhalb der Kategorien \"Repaints\", \"Tauschbörse\" und \"Sonstiges aller Art\" ist eine englische Beschreibung verpflichtend.<br>Falls du Hilfe bei der Übersetzung benötigst, helfen unsere <a href=\"https://reboot.omsi-webdisk.de/ticket/ticket-add/?categoryID=255\"><strong>Übersetzer (Ticketsystem)</strong></a> oder Online-Dienste wie <a href=\"https://www.deepl.com/translator/\" data-sc-dereferer=\"covered\"><strong>DeepL</strong></a>.<br><a href=\"https://reboot.omsi-webdisk.de/net/article/92-filebase-englische-beschreibungen/\"><strong>Ankündigung zu englischen Datei-Beschreibungen</strong></a></li>";
    const QString en = "<li><strong>English description:</strong> An English description is missing. For all files outside the categories \"Repaints\", \"Programs & Tools\" and \"Miscellaneous of all kinds\" an English description is necessary.<br>In case you need help with the translation, our <a href=\"https://reboot.omsi-webdisk.de/ticket/ticket-add/?categoryID=255\"><strong>Translators (Ticketing system)</strong></a> or online services like <a href=\"https://www.deepl.com/translator/\" data-sc-dereferer=\"covered\"><strong>DeepL</strong></a> can help you.<br><a href=\"https://reboot.omsi-webdisk.de/net/article/93-filebase-english-descriptions/\"><strong>Announcement on English file descriptions</strong></a></li>";
};

class cMissingDescription
{
public:
    const QString de = "<li><strong>Beschreibung:</strong> Es fehlt eine Beschreibung. Bitte füge deinem Datei-Eintrag eine Beschreibung hinzu, damit bei den Endbenutzern keine Unklarheiten aufkommen.</li>";
    const QString en = "<li><strong>Description:</strong> A description is missing. Please add a description to your file entry so that there is no ambiguity for the end users.</li>";
};

class cMissingPreviewPicture
{
public:
    const QString de = "<li><strong>Vorschaubild:</strong> Es fehlt ein Vorschaubild. Bei allen Datei-Einträgen, wo man etwas sehen kann, muss ein Vorschaubild vorhanden sein.</li>";
    const QString en = "<li><strong>Preview picture:</strong> A preview picture is missing. All file entries with something to see must have a preview picture.</li>";
};

class cExternalDownload
{
public:
    const QString de = "<li><strong>Externer Downloadlink:</strong> Du hast deine Datei auf einem externen Hoster hochgeladen. Bitte lade deine Datei direkt in der WebDisk hoch. Diese Regel geht aus einem Userentscheid im Mai 2020 heraus, alle Infos dazu findest du <a href=\"https://reboot.omsi-webdisk.de/community/thread/1069-externe-downloads-external-downloads/\"><strong>hier</strong></a>.<br>Überschreitet deine Datei die maximale Dateigröße, gib uns Bescheid. Dann laden wir dir dein Archiv gerne in deine Datei hoch.</li>";
    const QString en = "<li><strong>External download link:</strong> You have uploaded your file to an external hoster. Please upload your file directly to WebDisk. This rule is the result of a user decision in May 2020, you can find all the info about it <a href=\"https://reboot.omsi-webdisk.de/community/thread/1069-externe-downloads-external-downloads/\"><strong>here</strong></a>.<br>If your file is bigger than the maximum file size, please let us know. Then we will upload your archive to your file.</li>";
};

class cCopyrightviolation
{
public:
    // li ... br ol
    const QString deStart = "<li><strong>Urheberrechtsverletzung:</strong> In deinem Datei-Eintrag wird / werden eine / mehrere Urheberrechtsverletzung(en) vermutet.<ol>";
    const QString enStart = "<li><strong>Copyright violation:</strong> Your file entry is suspected of copyright violation(s).<ol>";

    // /ol ... /li
    const QString deEnd = "</ol>Bitte lege uns schriftliche Beweise (z.B. als Screenshot in bester Größe) vor, dass du zu den jeweiligen Bedingungen eine Zustimmung des Autors erhalten hast oder bearbeite deine Datei so, sodass die o.g. Gründe erfüllt werden.<br><a href=\"https://reboot.omsi-webdisk.de/net/article/1-was-besagen-die-lizenzen-copyrights/\"><strong>Infos zum Urheberrecht</strong></a></li>";
    const QString enEnd = "</ol>Please provide us with written evidence (e.g. as a screenshot in best size) that you have received the author's agreement to the respective conditions or edit your file in such a way that the above reasons are fulfilled.<br><a href=\"https://reboot.omsi-webdisk.de/net/article/2-what-do-the-listed-licenses-copyrights-mean/\"><strong>Copyright information</strong></a></li>";
};

class cInstallationinstruction
{
public:
    const QString de = "<li><strong>Anleitung:</strong> Bei deinem Datei-Eintrag handelt es sich um eine (Einbau)anleitung. Bitte veröffentliche diese im entsprechendem Forum <a href=\"https://reboot.omsi-webdisk.de/community/board/171-tutorials/\"><strong>Tutorials</strong></a>.";
    const QString en = "<li><strong>Instruction:</strong> Your file entry is an (installation) guide. Please publish them in the appropriate forum <a href=\"https://reboot.omsi-webdisk.de/community/board/171-tutorials/\"><strong>tutorials</strong></a>.";
};

class cArchiveDamaged
{
public:
    const QString de = "<li><strong>Archiv leer / beschädigt:</strong> Dein hochgeladenes Archiv in deinem Datei-Eintrag ist leer oder beschädigt. Bitte überprüfe dein gepacktes Archiv und lade es als neue Version in deinem Datei-Eintrag erneut hoch.<br>Für eine gute Kompressionsrate ohne Fehler empfehlen wird das kostenlose Programm <a href=\"https://www.7-zip.de/\"><strong>7-Zip</strong></a>.</li>";
    const QString en = "<li><strong>Archive empty / damaged:</strong> Your uploaded archive in your file entry is empty or damaged. Please check your packed archive and upload it again as a new version in your file entry.<br>For a good compression rate without errors we recommend the free program <a href=\"https://7-zip.org/\"><strong>7-Zip</strong></a>.</li>";
};

class cNoReference
{
public:
    const QString de = "<li><strong>Kein OMSI-Bezug:</strong> Dein Datei-Eintrag steht in keinem Bezug mit OMSI. In unserer Filebase sind nur Datei-Einträge erlaubt, welche dieses Kriterium erfüllen.</li>";
    const QString en = "<li><strong>No relation to OMSI:</strong> Your file entry is not related to OMSI. Only file entries that fulfill this criteria are allowed in our filebase.</li>";
};

class cNoValue
{
public:
    const QString de = "<li><strong>Datei ohne Mehrwert:</strong> Dein Datei-Eintrag hat keinen Mehrwert. In dieser Form hat deine hochgeladene Datei für Endbenutzer keinen Verwendungszweck.</li>";
    const QString en = "<li><strong>File without added value:</strong> Your file entry has no added value. In this form, your uploaded file has no purpose for end users.</li>";
};

class cContactdetails
{
public:
    const QString de = "<li><strong>Kontaktdaten:</strong> Wir haben die Kontaktdaten aus deinem Datei-Eintrag entfernt. Wir können nicht sicherstellen, ob dies wirklich deine Kontaktdaten sind. Abgesehen davon könnten diese Opfer von Spam und / oder Missbrauch werden.</li>";
    const QString en = "<li><strong>Contact information:</strong> We have removed the contact information from your file entry. We cannot ensure that this is really your contact information. Apart from that, they could become victims of spam and / or abuse.</li>";
};

class cTrademarkviolation
{
public:
    const QString de = "";
    const QString en = "";
};

class cProjectpresentation
{
public:
    const QString de = "<li><strong>Projektvorstellung:</strong> Offensichtlich möchtest du dein Projekt vorstellen. Dazu nutze bitte den <a href=\"https://reboot.omsi-webdisk.de/community/board/184-wip-in-entwicklung/\"><strong>Forenbereich für Projektvorstellungen</strong></a>. Dort kannst du dann ein neues Thema eröffnen und dein Projekt vorstellen. Bitte beachte, dass die Vorstellung auch Bilder beinhalten sollte, die du dort unter \"Dateianhänge und Bilder\" hochladen kannst.<br>Die Filebase ist nur für fertige Projekte bzw. für Projekte einem Verwendungszweck für den Endbenutzer gedacht.</li>";
    const QString en = "<li><strong>Project presentation:</strong> Obviously you want to present your project. For this please use the <a href=\"https://reboot.omsi-webdisk.de/community/board/184-wip-in-entwicklung/\"><strong>forum area for project presentations</strong></a>. There you can open a new topic and present your project. Please note that the presentation should also include pictures, which you can upload there under \"File attachments and pictures\".<br>The filebase is intended only for finished projects or for projects with a purpose for the end user.</li>";
};

class cScreenshot
{
public:
    const QString de = "<li><strong>Screenshot:</strong> Du hast nur ein Bild hochgeladen. Wenn du dein Werk nur zeigen möchtest, nutze bitte den <a href=\"https://reboot.omsi-webdisk.de/community/thread/465-screenshots-omsi/\"><strong>Screenshot-Thread für OMSI</strong></a>. Wenn du es in der Filebase für andere Benutzer hochladen möchtest, liefere bitte alle dafür notwendigen Dateien mit. Alle Archive müssen den vollständigen OMSI-Pfad enthalten.</li>";
    const QString en = "<li><strong>Screenshot:</strong> You have uploaded only a picture. If you want to show your creation only, please use the <a href=\"https://reboot.omsi-webdisk.de/community/thread/465-screenshots-omsi/\"><strong>screenshot thread for OMSI</strong></a>. If you want to upload it to the filebase for other users, please provide all necessary files. All archives must contain the complete OMSI path.</li>";
};

class cFooter4
{
public:
    const QString de = "<p><br></p><p>Wegen den obenstehenden Gründen wurde dein Datei-Eintrag gelöscht.</p><p><br></p><p>Viele Grüße";
    const QString en = "<p><br></p><p>Because of the above reasons your file entry has been deleted.</p><p><br></p><p>Best regards";
};

class cFooter3
{
public:
    const QString de = "<p><br></p><p>Wegen den oben stehenden Gründen wurde dein Datei-Eintrag gelöscht. Sie befindet sich in der Originalversion im Anhang.</p><p><br></p><p>Viele Grüße";
    const QString en = "<p><br></p><p>Because of the above reasons, your file entry has been deleted. You can find the original version in the attachment section.</p><p><br></p><p>Best regards";
};

class cFooter2
{
public:
    const QString de = "<p><br></p><p>Deine Datei befindet sich im Anhang. Es gibt für dich keine Admonitionen oder Sanktionen. Diese sind beim Wiederholungsfall jedoch nicht ausgeschlossen.</p><p><br></p><p>Viele Grüße";
    const QString en = "<p><br></p><p>Your file is attached. There are no warnings or sanctions for you. However, these are not excluded in case of repetition.</p><p><br></p><p>Best regards";
};

class cFooter1
{
public:
    const QString de = "<p><br></p><p>Sobald du deinen Datei-Eintrag <a href=\"https://reboot.omsi-webdisk.de/wiki/entry/64-hochladen-von-dateien-in-der-filebase/#4-Neue-Version-hinzufgen\"><strong>korrigiert / dein Archiv erneut hochgeladen</strong></a> hast, gib mir in dieser Konversation Bescheid und nach erneuter Prüfung wird dein Datei-Eintrag sehr gerne freigeschaltet.</p><p><br></p><p>Viele Grüße";
    const QString en = "<p><br></p><p>As soon as you <a href=\"https://reboot.omsi-webdisk.de/wiki/entry/85-uploading-files-to-the-filebase/#4-Add-new-version\"><strong>corrected / uploaded your archive again</strong></a> let me know in this conversation and after rechecking your file entry will be gladly unlocked.</p><p><br></p><p>Best regards";
};

///////////////////////////////////////
///////////////////////////////////////

class crHeader
{
public:
    const QString de = ",</p><p>ich kontaktiere dich bezüglich deiner folgenden Rezension unter der Datei:</p>";
    const QString en = "<p>Hello,</p><p>I am contacting you regarding your following review under the file:</p>";
};

class crMiddle
{
public:
    const QString de = "<p><br></p><p>Deine Rezension wurde gelöscht, da sie gegen den Regeln für Rezensionen verstößt.</p>";
    const QString en = "<p><br></p><p>Your review has been deleted because it does not comply with the rules for reviews.</p>";
};

class crRegeln
{
public:
    const QString de = "<woltlab-quote data-author=\"den Regeln für Rezensionen\" data-link=\"\"><ul><li>Eine Rezension sollte <strong>objektiv und beweisbar</strong> sein. Kritikpunkte sollten hinreichend erläutert werden, sodass sie für jeden nachprüfbar sind und vom Autor als <strong>Verbesserungsvorschlag</strong> aufgenommen werden können.</li><li><strong>Bezug</strong>: Losgelöst von persönlichem Empfinden sollte eine Rezension ausschließlich den Download an sich bewerten. Das Rezensionssystem bietet weder Platz für persönliche Differenzen noch für Reaktionen auf andere Rezensionen. <em>(Beispiel: Jemand schreibt eine positive Rezension nur aufgrund der Tatsache, dass jemand Anderes eine negative Rezension verfasst hatte)</em>.</li></ul><p>Bitte beachte, dass subjektive Kommentare oder andere Bemerkungen, die nicht in das obige Schema passen, gelöscht und bei Wiederholung ggf. sanktioniert werden. Für Kommentare aller Art steht es Dir jederzeit frei, die Kommentarfunktion zu nutzen. Wir fördern hier in der WebDisk ausdrücklich eine konstruktives Diskussionskultur, in der jeder seine Meinung zum Ausdruck bringen kann. Für Rezensionen gibt es allerdings höhere Hürden.</p></woltlab-quote><p><br></p>";
    const QString en = "<woltlab-quote data-author=\"the rules for Reviews\" data-link=\"\"><ul><li>A review should be <strong>objective and provable</strong>. Points of criticism should be sufficiently explained so that they are verifiable for everyone and can be accepted by the author as a <strong>suggestion for improvement</strong>.</li><li><strong>Reference</strong>: A review should only evaluate the download itself, regardless of personal feelings. The review system offers no room for personal differences or reactions to other reviews. <em>(Example: someone writes a positive review only because someone else wrote a negative review)</em>.</li></ul><p>Please note that subjective comments or other remarks that do not fit into the above scheme will be deleted and sanctioned if repeated. For comments of any kind you are always free to use the comment section. We expressly encourage a constructive culture of discussion here on the WebDisk, where everyone can express their opinion. However, there are higher hurdles for reviews.</p></woltlab-quote><p><br></p>";
};

///////////////////////////////////////
///////////////////////////////////////

class reasons
{
private:
    cFooter1 Footer1;
    cFooter2 Footer2;
    cFooter3 Footer3;
    cFooter4 Footer4;

public:
    cAdmonition Admonition;
    cHeader Header;
    cHeader2 Header2;
    cFilestructure Filestructure;
    cMissingPictures MissingPictures;
    cEnDescription EnDescription;
    cMissingDescription MissingDescription;
    cMissingPreviewPicture MissingPreviewPicture;
    cExternalDownload ExternalDownload;
    cCopyrightviolation Copyrightviolation;
    cProjectpresentation Projectpresentation;
    cScreenshot Screenshot;
    cInstallationinstruction Installationinstruction;
    cArchiveDamaged ArchiveDamaged;
    cNoReference NoReference;
    cNoValue NoValue;
    cContactdetails Contactdetails;
    cTrademarkviolation Trademarkviolation;


    QString getFooter(int index, QString language)
    {
        if (language == "DEU")
        {
            switch (index)
            {
                case 2:  return Footer2.de; break;
                case 3:  return Footer3.de; break;
                case 4:  return Footer4.de; break;
                default: return Footer1.de; break;
            }
        }
        else
        {
            switch (index)
            {
                case 2:  return Footer2.en; break;
                case 3:  return Footer3.en; break;
                case 4:  return Footer4.en; break;
                default: return Footer1.en; break;
            }
        }

        return "";
    }

    /////////////////////

    crHeader rHeader;
    crMiddle rMiddle;
    crRegeln rRegeln;
};

#endif // FBHFUNCTIONS_H
