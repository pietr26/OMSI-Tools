#ifndef OCFONT_H
#define OCFONT_H

#include "OCBase.h"

class Character
{
public:
    QString character;
    int leftPixel;
    int rightPixel;
    int highestPixelInFontRow;
};

class SingleFont
{
public:
    QList<Character*> characters;
    QString name;
    QString colorTexture;
    QString alphaTexture;
    int maxHeightOfChars;
    int distanceBetweenChars;
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

#endif // OCFONT_H
