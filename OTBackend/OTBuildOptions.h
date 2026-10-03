#ifndef OTBUILDOPTIONS_H
#define OTBUILDOPTIONS_H

#include <QString>

class OTBuildOptions
{
public:
    enum methods
    {
        Dev = 1,
        Alpha = 2,
        Beta = 3,
        EA = 4,
        Lite = 5,
        Prerelease = 6,
        Release = 7
    };
    Q_DECLARE_FLAGS(buildOptions, methods)

    static QString getBuildName(buildOptions flag)
    {
        switch (flag)
        {
        case Dev:           return "Dev";           break;
        case Alpha:         return "Alpha";         break;
        case Beta:          return "Beta";          break;
        case EA:            return "Early Access";  break;
        case Lite:          return "Lite";          break;
        case Prerelease:    return "Prerelease";    break;
        case Release:       return "Release";       break;
        default:            return "unknown";       break;
        }
    }
};

#endif // OTBUILDOPTIONS_H
