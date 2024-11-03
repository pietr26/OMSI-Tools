#ifndef OCBASE_H
#define OCBASE_H

#include "OTBackend/OTGlobal.h"

namespace OCBase
{

QString writeFileHeader();

class Unknown
{
    // just a placeholder for any unknown fields.
};

class File // Class for all classes with [optional] (directly) readable and/or writeable files - inherit as public!
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

    void processFileIOResponse(FileIOResponse response, QString filename = "", QString errorString = "");

    QPair<FileIOResponse, QString> generateErrPair(FileIOResponse responseCode, QString errorString) { return QPair<FileIOResponse, QString>(responseCode, errorString); }

    virtual FileIOResponse read() { return FileIOResponse::errFunctionNotDefined; } // override (pls mark). For error code information take a look in source class
    virtual FileIOResponse write() { return FileIOResponse::errFunctionNotDefined; } // override (pls mark). For error code information take a look in source class

    virtual void clear() { } // override (pls mark).

    QList<QPair<FileIOResponse, QString>> errors; // TODO: error stack for minor errors while last reading / writing process - maybe use another template class for list
};

}

namespace OCType
{

template<class T>
class Coord2D
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    Coord2D() { }
    Coord2D(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    T x, y;
};

template<class T>
class Coord2DSide
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    Coord2DSide() { }
    Coord2DSide(T y, T z)
    {
        this->y = y;
        this->z = z;
    }

    T y, z;
};

template<class T>
class Coord3D
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    Coord3D() { }
    Coord3D(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    T x, y, z;
};

template<class T>
class Box
{
public:
    static_assert(std::is_arithmetic<T>::value || std::is_same_v<T, QVariant>, "The type T must be numeric.");

    Box() { }
    Box(T xSize, T ySize, T zSize, T xPos, T yPos, T zPos)
    {
        this->xSize = xSize;
        this->ySize = ySize;
        this->zSize = zSize;
        this->xPos = xPos;
        this->yPos = yPos;
        this->zPos = zPos;
    }

    T xSize, ySize, zSize;
    T xPos, yPos, zPos;
};

}

#endif // OCBASE_H
