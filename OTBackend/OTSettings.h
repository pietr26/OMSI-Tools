#ifndef OTSETTINGS_H
#define OTSETTINGS_H

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QPair>
#include <QSettings>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QTranslator>
#include <QVariant>
#include <QWidget>

#include "OTInformation.h"
#include "OTPath.h"
#include "OTPlatform.h"

class OTSettings
{
public:
    void write(QString module, QString name, QVariant value)
    {
        QSettings preferences(OTInformation::name, module);
        preferences.setValue(name, value);
        qDebug().noquote().nospace() << "Write pref to " << module << ": "<< name << ", value: " << value;
    }

    QVariant read(QString module, QString name, bool logging = true)
    {
        QSettings preferences(OTInformation::name, module);
        QVariant value = preferences.value(name);
        if (logging) qDebug().noquote().nospace() << "Read pref from " << module << ": "<< name << ", value: " << value;

        return value;
    }

    void remove(QString module, QString name) { QSettings(OTInformation::name, module).remove(name); }

    void removeAll() { OTPlatform::removeAllPreferences(OTInformation::name); }

    bool checkMainDir(QWidget *parent, QString mainDir, bool openMessage)
    {
        if (!mainDir.isEmpty() && !OTPath::exists(mainDir, "Omsi.exe"))
        {
            qWarning().noquote() << "'" + mainDir + "' isn't an OMSI path!";

            if (openMessage)
            {
                QMessageBox::StandardButton reply = QMessageBox::warning(parent, QObject::tr("Could not found \"Omsi.exe\""), QObject::tr("'Omsi.exe' could not found in the selected directory. Is it the correct path? Otherwise, problems may appear in some modules. Should a new path be selected?"), QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                    getOmsiPath(parent);
            }

            return false;
        }

        return !mainDir.isEmpty();
    }

    QString getOmsiPath(QWidget *parent, bool openMessage = true, QString path = "")
    {
        if (path.isEmpty())
            path = OTPlatform::omsiInstallationGuess();

        QString mainDir = QFileDialog::getExistingDirectory(parent, QObject::tr("Select the OMSI main directory..."), path);

        if (openMessage) checkMainDir(parent, mainDir, openMessage);

        return mainDir;
    }

    QList<QPair<QString, QVariant>> getAllPreferences()
    {
        return OTPlatform::allPreferences(OTInformation::name);
    }

    QString getAllPreferencesFormatted()
    {
        QString returnString = "\n";

        foreach (auto current, getAllPreferences())
            returnString += current.first + " -> " + current.second.toString() + "\n";

        return returnString;
    }

    void loadTranslator(QString contextIdentifier = "")
    {
        QTranslator *translator = new QTranslator();
        QTranslator *baseTranslator = new QTranslator();

        QString languageFile;
        QString baseLanguageFile;

        switch (read("main", "language").toInt())
        {
            case  0: languageFile = "tr_en"; baseLanguageFile = "qtbase_en"; break;
            case  1: languageFile = "tr_de"; baseLanguageFile = "qtbase_de"; break;
            case  2: languageFile = "tr_fr"; baseLanguageFile = "qtbase_fr"; break;
            case  3: languageFile = "tr_cs"; baseLanguageFile = "qtbase_cs"; break;
            case  4: languageFile = "tr_nl"; baseLanguageFile = "qtbase_nl"; break;

            case  7: languageFile = "tr_fi"; baseLanguageFile = "qtbase_fi"; break;
            case  9: languageFile = "tr_ja"; baseLanguageFile = "qtbase_ja"; break;
        }

        if (languageFile != "")
        {
            bool trLoad = translator->load(languageFile, ":/rec/data/translations" + contextIdentifier + "/");
            bool trInstall = qApp->installTranslator(translator);

            bool baseTrLoad = baseTranslator->load(baseLanguageFile, ":/rec/data/translationsBASE/");
            bool baseTrInstall = qApp->installTranslator(baseTranslator);

            if (trLoad && trInstall && baseTrLoad && baseTrInstall)
                qInfo() << "Loaded translations";
            else
            {
                qWarning() << "Error while loading translations!";

                qDebug().noquote() << "trLoad:" << trLoad;
                qDebug().noquote() << "trInstall:" << trInstall;
                qDebug().noquote() << "baseTrLoad:" << baseTrLoad;
                qDebug().noquote() << "baseTrInstall:" << baseTrInstall;
            }
        }
    }

    void setPreferencesOnStart()
    {
        if (!read("main", "autoUpdateCheck").isValid())
            write("main", "autoUpdateCheck", 2);

        if (!read("main", "logfileMode").isValid())
            write("main", "logfileMode", 0);

        if (!read("main", "autosave").isValid())
            write("main", "autosave", true);

        bool themeOk = false;
        read("main", "theme").toString().toInt(&themeOk);

        if (!read("main", "theme").isValid() || themeOk)
            write("main", "theme", "Fusion");

        if (!read("main", "autosaveDuration").isValid())
            write("main", "autosaveDuration", 60);

        if (!read("main", "useDiscordGameSDK").isValid())
            write("main", "useDiscordGameSDK", true);

        if (!read("wStart", "messagesVisible").isValid())
            write("wStart", "messagesVisible", true);

        if (!read("wFonts", "texPreview").isValid())
            write("wFonts", "texPreview", 1);

        const QString texconv = OTPlatform::texconvExecutable();
        if (!texconv.isEmpty() && !QFile(texconv).exists()) qInfo() << "Extract textconv.exe:" << QFile().copy(":/rec/data/external/texconv.exe", texconv);

        if (!QFile(OTPlatform::applicationFile("_docs/Handbuch DE.pdf")).exists())
        {
            QDir().mkdir(OTPlatform::applicationFile("_docs"));
            qInfo() << "Extract manuals:" << QFile().copy(":/rec/data/manual/Handbuch DE.pdf", OTPlatform::applicationFile("_docs/Handbuch DE.pdf"));
        }

        write("wVerifyMap", "advVerifying", false);
    }

    void setPreferencesOnStartWDT()
    {
        bool themeWDTOk = false;
        read("WDT", "theme").toString().toInt(&themeWDTOk);

        if (!read("WDT", "theme").isValid() || themeWDTOk)
            write("WDT", "theme", "Fusion");

        if (!read("WDT", "welcomeDE").isValid())
            write("WDT", "welcomeDE", "Hallo");

        if (!read("WDT", "welcomeEN").isValid())
            write("WDT", "welcomeEN", "Hello");
    }

    void setPreferencesOnStartLFC()
    {
        bool themeLFCOk = false;
        read("LFC", "theme").toString().toInt(&themeLFCOk);

        if (!read("LFC", "language").isValid())
            write("LFC", "language", 1);

        if (!read("LFC", "theme").isValid() || themeLFCOk)
            write("LFC", "theme", "Fusion");
    }

    QString getCurrentLanguageCode()
    {
        int index = read("main", "language").toInt();

        switch (index) {
            case 0: return "en"; break;
            case 1: return "de"; break;
            case 2: return "fr"; break;
            case 4: return "cz"; break;
            default: return "err";
        }
    }

    bool devModeEnabled()
    {
        if (OTInformation::build != OTBuildOptions::Dev) return read("main", "devToolsEnabled").toBool();

        return true;
    }

    void loadBaseStyleheet()
    {
        QString stylesheet;

        // set default (better) height:
        if (QApplication::style()->name() == "windows11") stylesheet += "QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QDateTimeEdit, QDateEdit, QTimeEdit, QButton { height: 25px; }";

        qApp->setStyleSheet(stylesheet);
    }


    static Qt::ColorScheme currentColorScheme() {
        OTSettings set;
        if(set.read("main", "theme").toString() == "windowsvista")
            return Qt::ColorScheme::Light;
        else
            return QApplication::styleHints()->colorScheme();
    }
};

#endif // OTSETTINGS_H
