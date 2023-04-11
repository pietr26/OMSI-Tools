#ifndef OTOMSIFILEHANDLER_MODELS_H
#define OTOMSIFILEHANDLER_MODELS_H

#include "OTGlobal.h"

class OTVerifyStuff
{
public:
    class OTVerifyStuffMissing
    {
    public:
        QStringList tiles;
        QStringList textures;
        QStringList globalTextures;
        QStringList sceneryobjects;
        QStringList splines;
        QStringList vehicles;
        QStringList humans;

        QStringList getAll()
        {
            QStringList all;
            all << tiles;
            all << textures;
            all << globalTextures;
            all << sceneryobjects;
            all << splines;
            all << vehicles;
            all << humans;

            return all;
        }

        void removeDuplicates()
        {
            tiles.removeDuplicates();
            textures.removeDuplicates();
            globalTextures.removeDuplicates();
            sceneryobjects.removeDuplicates();
            splines.removeDuplicates();
            vehicles.removeDuplicates();
            humans.removeDuplicates();
        }

        void clear()
        {
            tiles.clear();
            textures.clear();
            globalTextures.clear();
            sceneryobjects.clear();
            splines.clear();
            vehicles.clear();
            humans.clear();
        }
    };

    class OTVerifyStuffExisting
    {
    public:
        QStringList tiles;
        QStringList textures;
        QStringList globalTextures;
        QStringList sceneryobjects;
        QStringList splines;
        QStringList vehicles;
        QStringList humans;

        QStringList getAll()
        {
            QStringList all;
            all << tiles;
            all << textures;
            all << globalTextures;
            all << sceneryobjects;
            all << splines;
            all << vehicles;
            all << humans;

            return all;
        }

        void removeDuplicates()
        {
            tiles.removeDuplicates();
            textures.removeDuplicates();
            globalTextures.removeDuplicates();
            sceneryobjects.removeDuplicates();
            splines.removeDuplicates();
            vehicles.removeDuplicates();
            humans.removeDuplicates();
        }

        void clear()
        {
            tiles.clear();
            textures.clear();
            globalTextures.clear();
            sceneryobjects.clear();
            splines.clear();
            vehicles.clear();
            humans.clear();
        }
    };

    OTVerifyStuffMissing missing;
    OTVerifyStuffExisting existing;

    void clear()
    {
        missing.clear();
        existing.clear();
    }

    void removeDuplicates()
    {
        missing.removeDuplicates();
        existing.removeDuplicates();
    }
};

/// Contains all informations about a font character
class OTCharacterModel
{
public:
    OTCharacterModel(QString ca = "", int lP = -1, int rP = -1, int hP = -1, QString co = "")
    {
        character = ca;
        leftPixel = lP;
        rightPixel = rP;
        highestPixelInFontRow = hP;
        comment = co;
    }

    QString character;
    int leftPixel;
    int rightPixel;
    int highestPixelInFontRow;
    QString comment;
};

/// Contains all informations about a font
class OTFontModel
{
public:
    bool error = false;
    bool moreThanOneFont = false;

    QString path;
    QString name;
    QString colorTexture;
    QString alphaTexture;
    int maxHeightOfChars = -1;
    int distanceBetweenChars = -1;

    QList<OTCharacterModel> charList;

    void clear()
    {
        error = false;
        path.clear();
        name.clear();
        colorTexture.clear();
        alphaTexture.clear();
        maxHeightOfChars = int();
        distanceBetweenChars = int();
        charList.clear();
    }
};







#endif // OTOMSIFILEHANDLER_MODELS_H
