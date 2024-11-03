#include "OCFont.h"


QString Character::character() const
{
    return _character;
}

void Character::setCharacter(const QString &newCharacter)
{
    _character = newCharacter;
}

int Character::leftPixel() const
{
    return _leftPixel;
}

void Character::setLeftPixel(int newLeftPixel)
{
    _leftPixel = newLeftPixel;
}

int Character::rightPixel() const
{
    return _rightPixel;
}

void Character::setRightPixel(int newRightPixel)
{
    _rightPixel = newRightPixel;
}

int Character::highestPixelInFontRow() const
{
    return _highestPixelInFontRow;
}

void Character::setHighestPixelInFontRow(int newHighestPixelInFontRow)
{
    _highestPixelInFontRow = newHighestPixelInFontRow;
}

QList<Character *> SingleFont::characters() const
{
    return _characters;
}

void SingleFont::setCharacters(const QList<Character *> &newCharacters)
{
    _characters = newCharacters;
}

void SingleFont::appendCharacter(Character *newCharacter)
{
    _characters.append(newCharacter);
}

void SingleFont::insertCharacter(int pos, Character *newCharacter)
{
    _characters.insert(pos, newCharacter);
}

void SingleFont::moveCharacter(int from, int to)
{
    _characters.move(from, to);
}

void SingleFont::removeCharacter(int pos, int n)
{
    _characters.remove(pos, n);
}

QString SingleFont::name() const
{
    return _name;
}

void SingleFont::setName(const QString &newName)
{
    _name = newName;
}

QString SingleFont::colorTexture() const
{
    return _colorTexture;
}

void SingleFont::setColorTexture(const QString &newColorTexture)
{
    _colorTexture = newColorTexture;
}

QString SingleFont::alphaTexture() const
{
    return _alphaTexture;
}

void SingleFont::setAlphaTexture(const QString &newAlphaTexture)
{
    _alphaTexture = newAlphaTexture;
}

int SingleFont::maxHeightOfChars() const
{
    return _maxHeightOfChars;
}

void SingleFont::setMaxHeightOfChars(int newMaxHeightOfChars)
{
    _maxHeightOfChars = newMaxHeightOfChars;
}

int SingleFont::distanceBetweenChars() const
{
    return _distanceBetweenChars;
}

void SingleFont::setDistanceBetweenChars(int newDistanceBetweenChars)
{
    _distanceBetweenChars = newDistanceBetweenChars;
}

OCBase::File::FileIOResponse FontCollection::read()
{
    QFile file(_path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        clear();
        return FileIOResponse::errFileNotOpen;
    }

    QTextStream in(&file);
    in.setEncoding(_encoding);
    qDebug().noquote() << "Font encoding:" << _encoding;
    QString line;

    try
    {
        while (!in.atEnd())
        {
            line = in.readLine();

            if (line == "[newfont]")
            {
                _fonts.append(new SingleFont());
                _fonts.last()->setName(in.readLine().toUtf8());
                _fonts.last()->setColorTexture(in.readLine().toUtf8());
                _fonts.last()->setAlphaTexture(in.readLine().toUtf8());
                _fonts.last()->setMaxHeightOfChars(in.readLine().toUtf8().toInt());
                _fonts.last()->setDistanceBetweenChars(in.readLine().toUtf8().toInt());
            }
            else if (line == "[char]")
            {
                Character *character = new Character();

                character->setCharacter(in.readLine().toUtf8());
                character->setLeftPixel(in.readLine().toUtf8().toInt());
                character->setRightPixel(in.readLine().toUtf8().toInt());
                character->setHighestPixelInFontRow(in.readLine().toUtf8().toInt());

                if (!_fonts.isEmpty()) _fonts.last()->characters().append(character);
                else return FileIOResponse::errMinor;
            }
        }

        file.close();
    }
    catch (...)
    {
        file.close();
        clear();
        return FileIOResponse::errCritical;
    }

    return FileIOResponse::valid;
}

OCBase::File::FileIOResponse FontCollection::write()
{
    QFile file(_path);

    if (!file.open(QFile::WriteOnly | QFile::Text)) { return FileIOResponse::errFileNotOpen; }

    QTextStream out(&file);

    out.setEncoding(QStringConverter::Latin1);

    try {
        OTSettings set;

        out << OCBase::writeFileHeader();
        out << "Author: " << set.read("main", "author").toString() << "\n";
        out << "Font count: " << _fonts.count() << "\n";
        out << "Total characters: " << totalCharacterCount() << "\n";
        out << "\n";

        foreach (SingleFont *font, _fonts) {
            out << "#############################################" << "\n";
            out << "Font name: " << font->name() << "\n";
            out << "Characters: " << font->characters().count() << "\n";
            out << "\n";

            out << "[newfont]" << "\n";
            out << font->name() << "\n";
            out << font->colorTexture() << "\n";
            out << font->alphaTexture() << "\n";

            QString maxHeightOfChars = (font->maxHeightOfChars() == -1) ? "" : QString::number(font->maxHeightOfChars());
            out << maxHeightOfChars << "\n";
            QString distanceBetweenChars = (font->distanceBetweenChars() == -1) ? "" : QString::number(font->distanceBetweenChars());
            out << distanceBetweenChars << "\n";
            out << "\n";

            foreach (Character *character, font->characters())
            {
                out << "[char]" << "\n";
                out << character->character() << "\n";
                out << character->leftPixel() << "\n";
                out << character->rightPixel() << "\n";
                out << character->highestPixelInFontRow() << "\n";
                out << "\n";
            }
        }

        file.close();
    }
    catch (...)
    {
        file.close();
        return FileIOResponse::errCritical;
    }

    return FileIOResponse::valid;
}

void FontCollection::clear() { _fonts.clear(); _path.clear(); _encoding = QStringConverter::Latin1; }

int FontCollection::totalCharacterCount()
{
    int characterCount = 0;
    foreach (SingleFont* font, _fonts) characterCount += font->characters().count();
    return characterCount;
}

QList<SingleFont *> FontCollection::fonts() const
{
    return _fonts;
}

void FontCollection::setFonts(const QList<SingleFont *> &newFonts)
{
    _fonts = newFonts;
}

void FontCollection::appendFont(SingleFont *singleFont)
{
    _fonts.append(singleFont);
}

void FontCollection::insertFont(int pos, SingleFont *singleFont)
{
    _fonts.insert(pos, singleFont);
}

void FontCollection::moveFont(int from, int to)
{
    _fonts.move(from, to);
}

void FontCollection::removeFont(int pos, int n)
{
    _fonts.remove(pos, n);
}

QString FontCollection::path() const
{
    return _path;
}

void FontCollection::setPath(const QString &newPath)
{
    _path = newPath;
}

QStringConverter::Encoding FontCollection::encoding() const
{
    return _encoding;
}

void FontCollection::setEncoding(QStringConverter::Encoding newEncoding)
{
    _encoding = newEncoding;
}

QMap<FontCollection::Selection, int> FontCollection::selection() const
{
    return _selection;
}

void FontCollection::setSelection(const QMap<Selection, int> &newSelection)
{
    _selection = newSelection;
}

void FontCollection::appendSelection(Selection selection, int pos)
{
    _selection.insert(selection, pos);
}
