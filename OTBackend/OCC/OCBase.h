#ifndef OCBASE_H
#define OCBASE_H

#include "OTBackend/OTGlobal.h"

class OCBase
{
public:
    // TODO: Copy from OTGlobal[OTFileOperations] - move all OMSI-related stuff to here
    /// Returns an universal file header
    QString writeFileHeader() { return "File created with " + OTInformation::name + " " + OTInformation::versions::currentVersion.first + " on " + misc.getDate() + ", " + misc.getTime() + "\n\n"; }

private:
    OTMiscellaneous misc;
};

class OCUnknown
{
    // just a placeholder for any unknown fields.
};

template<class T>
class OC2DCoordinates
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    OC2DCoordinates() { }
    OC2DCoordinates(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    T x;
    T y;
};

template<class T>
class OC2DCoordinatesSide
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    OC2DCoordinatesSide() { }
    OC2DCoordinatesSide(T y, T z)
    {
        this->y = y;
        this->z = z;
    }

    T y;
    T z;
};

template<class T>
class OC3DCoordinates
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    OC3DCoordinates() { }
    OC3DCoordinates(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    T x;
    T y;
    T z;
};

template<class T>
class OC3DBox
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    OC3DBox() { }
    OC3DBox(T xSize, T ySize, T zSize, T xPos, T yPos, T zPos)
    {
        this->xSize = xSize;
        this->ySize = ySize;
        this->zSize = zSize;
        this->xPos = xPos;
        this->yPos = yPos;
        this->zPos = zPos;
    }

    T xSize;
    T ySize;
    T zSize;
    T xPos;
    T yPos;
    T zPos;
};

class OCFile // Class for all classes with [optional] (directly) readable and/or writeable files - inherit as public!
{
public:
    enum FileIOResponse
    {
        errFunctionNotDefined = -3,
        errFileNotOpen = -2,
        errFileDoesntExist = -1,
        valid = 0,
        errMinor = 1,
        errCritical = 2
    };

    void processFileIOResponse(FileIOResponse response, QString filename = "", QString errorString = "")
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

    QPair<FileIOResponse, QString> generateErrPair(FileIOResponse responseCode, QString errorString) { return QPair<FileIOResponse, QString>(responseCode, errorString); }

    virtual FileIOResponse read() { return FileIOResponse::errFunctionNotDefined; } // override (pls mark). For error code information take a look in source class
    virtual FileIOResponse write() { return FileIOResponse::errFunctionNotDefined; } // override (pls mark). For error code information take a look in source class

    virtual void clear() { } // override (pls mark).

    QList<QPair<FileIOResponse, QString>> errors; // TODO: error stack for minor errors while last reading / writing process - maybe use another template class for list
};

#endif // OCBASE_H
