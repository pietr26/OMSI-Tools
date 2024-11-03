#ifndef OCFONT_H
#define OCFONT_H

#include "OCBase.h"

class Character
{
public:
    QString character() const;
    void setCharacter(const QString &newCharacter);
    int leftPixel() const;
    void setLeftPixel(int newLeftPixel);
    int rightPixel() const;
    void setRightPixel(int newRightPixel);
    int highestPixelInFontRow() const;
    void setHighestPixelInFontRow(int newHighestPixelInFontRow);

private:
    QString _character;
    int _leftPixel;
    int _rightPixel;
    int _highestPixelInFontRow;
};

class SingleFont
{
public:
    QList<Character *> characters() const;
    void setCharacters(const QList<Character *> &newCharacters);
    void appendCharacter(Character *newCharacter);
    void insertCharacter(int pos, Character *newCharacter);
    void moveCharacter(int from, int to);
    void removeCharacter(int pos, int n = 1);
    QString name() const;
    void setName(const QString &newName);
    QString colorTexture() const;
    void setColorTexture(const QString &newColorTexture);
    QString alphaTexture() const;
    void setAlphaTexture(const QString &newAlphaTexture);
    int maxHeightOfChars() const;
    void setMaxHeightOfChars(int newMaxHeightOfChars);
    int distanceBetweenChars() const;
    void setDistanceBetweenChars(int newDistanceBetweenChars);

private:
    QList<Character*> _characters;
    QString _name;
    QString _colorTexture;
    QString _alphaTexture;
    int _maxHeightOfChars;
    int _distanceBetweenChars;
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

    QList<SingleFont *> fonts() const;
    void setFonts(const QList<SingleFont *> &newFonts);
    void appendFont(SingleFont *singleFont);
    void insertFont(int pos, SingleFont *singleFont);
    void moveFont(int from, int to);
    void removeFont(int pos, int n = 1);
    QString path() const;
    void setPath(const QString &newPath);
    QStringConverter::Encoding encoding() const;
    void setEncoding(QStringConverter::Encoding newEncoding);

    QMap<Selection, int> selection() const;
    void setSelection(const QMap<Selection, int> &newSelection);
    void appendSelection(Selection selection, int pos);

private:
    QList<SingleFont*> _fonts;
    QString _path;
    QStringConverter::Encoding _encoding;

    QMap<Selection, int> _selection;
};

#endif // OCFONT_H
