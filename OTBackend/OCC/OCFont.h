#ifndef OCFONT_H
#define OCFONT_H


#include <QList>
#include <QMap>
#include <QString>
#include <QStringConverter>
#include "OCBase.h"

namespace OCFont {

class Character
{
public:
    QString character;
    int leftPixel = 0;
    int rightPixel = 0;
    int highestPixelInFontRow = 0;
};

class SingleFont
{
public:
    QList<Character*> characters;
    QString name;
    QString colorTexture;
    QString alphaTexture;
    int maxHeightOfChars = 0;
    int distanceBetweenChars = 0;
};

class FontCollection : public OCBase::File // oft
{
public:
    FileIOResponse read() override;

    FileIOResponse write() override;

    virtual void clear() override;

    int totalCharacterCount();

    enum Selection
    {
        FontSelection,
        CharacterSelection
    };

    QList<SingleFont*> fonts;
    QString path;
    QStringConverter::Encoding encoding;

    QMap<Selection, int> selection;
};

}



#endif // OCFONT_H
