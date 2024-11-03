#include "OCBase.h"

QString OCBase::writeFileHeader()
{
    return "File created with " + OTInformation::name + " " + OTInformation::versions::currentVersion.first + " on " + QDateTime::currentDateTime().toString("dd.MM.yyyy, hh:mm") + "\n\n";
}

void OCBase::File::processFileIOResponse(FileIOResponse response, QString filename, QString errorString)
{
    switch (response)
    {
    default: qDebug() << "FileIO response: No response code given."; break;

    case -3: qWarning() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": Virtual function is not defined."; break;
    case -2: qWarning() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": Cannot open file."; break;
    case -1: qWarning() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": File does not exist."; break;
    case 0: qInfo() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + " successful."; break;
    case 1: qWarning() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": Minor error" + ((!errorString.isEmpty()) ? " (" + errorString + ")": ""); break;
    case 2: qCritical() << "FileIO" + ((!filename.isEmpty()) ? " of " + filename : "") + ": Minor error" + ((!errorString.isEmpty()) ? " (" + errorString + ")": ""); break;
    }
}
