#ifndef OTINFORMATION_H
#define OTINFORMATION_H

#include <QList>
#include <QPair>
#include <QString>

#include "OTBuildOptions.h"

class OTInformation
{
public:
    inline static const QString name = "OMSI-Tools";
    inline static const OTBuildOptions::buildOptions build = OTBuildOptions::Dev;
    inline static const QString sourceCodeLength = "34.905";

    class versions
    {
    public:
        inline static const QList<QPair<QString, unsigned int>> allVersions = {
            //QPair<QString, unsigned int>("1.2.0", 40), // without "lite"!
            //QPair<QString, unsigned int>("1.2.0-beta", 39),
            QPair<QString, unsigned int>("1.2.0-dev", 38),
            QPair<QString, unsigned int>("1.1.2-lite", 37),
            QPair<QString, unsigned int>("1.1.2-beta", 36),
            QPair<QString, unsigned int>("1.1.2-dev", 35),
            QPair<QString, unsigned int>("1.1.1-lite", 34),
            QPair<QString, unsigned int>("1.1.0-lite", 33),
            QPair<QString, unsigned int>("1.1.0-beta", 32),
            QPair<QString, unsigned int>("1.1.0-dev", 31),
            QPair<QString, unsigned int>("1.0.0-lite", 30),
            QPair<QString, unsigned int>("1.0.0-beta", 29),
            QPair<QString, unsigned int>("1.0.0-dev", 28),
            QPair<QString, unsigned int>("0.10.1-lite", 27),
            QPair<QString, unsigned int>("0.10.1-beta", 26),
            QPair<QString, unsigned int>("0.10.1-dev", 25),
            QPair<QString, unsigned int>("0.10.0-lite", 24),
            QPair<QString, unsigned int>("0.10.0-prerelease", 23),
            QPair<QString, unsigned int>("0.10.0-beta", 22),
            QPair<QString, unsigned int>("0.10.0-dev", 21),
            QPair<QString, unsigned int>("0.9.0-beta", 20),
            QPair<QString, unsigned int>("0.9.0-dev", 19),
            QPair<QString, unsigned int>("0.8.0-beta", 18),
            QPair<QString, unsigned int>("0.8.0-dev", 17),
            QPair<QString, unsigned int>("0.7.0-beta", 16),
            QPair<QString, unsigned int>("0.7.0-dev", 15),
            QPair<QString, unsigned int>("0.6.0-beta", 14),
            QPair<QString, unsigned int>("0.6.0-dev", 13),
            QPair<QString, unsigned int>("0.5.0-beta", 12),
            QPair<QString, unsigned int>("0.5.0-dev", 11),
            QPair<QString, unsigned int>("0.4.0-beta", 10),
            QPair<QString, unsigned int>("0.4.0-dev", 9),
            QPair<QString, unsigned int>("0.3.0-beta", 8),
            QPair<QString, unsigned int>("0.3.0-dev", 7),
            QPair<QString, unsigned int>("0.2.1-beta", 6),
            QPair<QString, unsigned int>("0.2.1-dev", 5),
            QPair<QString, unsigned int>("0.2.0-beta", 4),
            QPair<QString, unsigned int>("0.2.0-dev", 3),
            QPair<QString, unsigned int>("0.1.0-beta", 2),
            QPair<QString, unsigned int>("0.1.0-dev", 1),
            QPair<QString, unsigned int>("0.0.0-dev", 0)
        };

        inline static const QPair<QString, unsigned int> currentVersion = allVersions.at(0);
    };
};

#endif // OTINFORMATION_H
