#ifndef OTOMSIFILEHANDLER_MODELS_H
#define OTOMSIFILEHANDLER_MODELS_H

#include <QString>
#include <QStringList>

class OTVerifyStuff
{
public:
    class OTVerifyStuffClass
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

        void toBackslash()
        {
            tiles.replaceInStrings("/", "\\");
            textures.replaceInStrings("/", "\\");
            globalTextures.replaceInStrings("/", "\\");
            sceneryobjects.replaceInStrings("/", "\\");
            splines.replaceInStrings("/", "\\");
            vehicles.replaceInStrings("/", "\\");
            humans.replaceInStrings("/", "\\");
        }
    };

    OTVerifyStuffClass missing;
    OTVerifyStuffClass existing;

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

    void toBackslash()
    {
        missing.toBackslash();
        existing.toBackslash();
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
