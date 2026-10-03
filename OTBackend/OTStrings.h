#ifndef OTSTRINGS_H
#define OTSTRINGS_H

#include <QList>
#include <QObject>
#include <QPair>
#include <QStringList>
#include <QStyleFactory>

#include "OTInformation.h"
#include "OTLinks.h"
#include "OTSettings.h"

class OTStrings
{
public:
    // Multilanguage strings
    static inline const QString langCs = "čeština (Czech)";
    static inline const QString langEn = "English";
    static inline const QString langFi = "suomi (Finnish)";
    static inline const QString langFr = "français (French)";
    static inline const QString langDe = "Deutsch (German)";
    static inline const QString langIt = "Italiano (Italian)";
    static inline const QString langJa = "日本語 (Japanese)";
    static inline const QString langNl = "Nederlands (Dutch)";

    static inline const QString textureSuffixes = QObject::tr("Images") + " (*.png *.jpg *.jpeg *.bmp *.dds)";

    static QList<QPair<QString, unsigned int>> getFunFacts()
    {
        OTSettings set;

        return QList<QPair<QString, unsigned int>> {
            // Link HTML: <a href='LINK'>TEXT</a>
            QPair<QString, unsigned int>(QObject::tr("The source code of %1 is about %2 lines long.").arg(OTInformation::name, OTInformation::sourceCodeLength), 8000),
            QPair<QString, unsigned int>(QObject::tr("%1 was born from a simple console application called \"Font Creator\".").arg(OTInformation::name), 10000),
            QPair<QString, unsigned int>(QObject::tr("With the updater of %1 you can install updates with one click. It is available through the preferences.").arg(OTInformation::name), 13000),
            QPair<QString, unsigned int>(QObject::tr("In the <a href='%2'>Wiki of %1</a> you can find useful explanations about all topics.", "Copy whole source text to prevent translations faults in HTML code").arg(OTInformation::name, OTLinks::Wiki::general), 11000),
            QPair<QString, unsigned int>(QObject::tr("Check out the latest developments in the <a href='%1'>presentation thread in the OMSI-WebDisk</a>.", "Copy whole source text to prevent translations faults in HTML code").arg(OTLinks::showroom), 11000),
            QPair<QString, unsigned int>(QObject::tr("Your hard disk is crowded? Clean up your main directory with %1' cleanup tool.").arg(OTInformation::name), 10000),
            QPair<QString, unsigned int>(QObject::tr("You've started %1 about %2 times.").arg(OTInformation::name, set.read("main", "startCount").toString()), 8000)
        };
    };

    static QString serverMaintenance() { return QObject::tr("The application server is currently undergoing maintenance (HTTP 503). Please try again later."); }

    static QList<QPair<QString, QString>> getThemeNames()
    {
        QStringList styles = QStyleFactory::keys();
        QList<QPair<QString, QString>> themes;

        for (int i = 0; i < styles.count(); i++)
        {
            themes.append(QPair<QString, QString>(styles[i], ""));
            if (styles[i] == "windows11")
            {
                themes[i].second = "Windows 11";
            }
            else if (styles[i] == "windowsvista")
            {
                themes[i].second = QObject::tr("Windows standard");
            }
            else if (styles[i] == "Windows")
            {
                themes[i].second = "Windows XP";
            }
            else if (styles[i] == "Fusion")
            {
                themes[i].second = "Fusion";
            }
            else
            {
                themes[i].second = themes[i].first;
            }
        }

        return themes;
    }
};

#endif // OTSTRINGS_H
