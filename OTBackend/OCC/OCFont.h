#ifndef OCFONT_H
#define OCFONT_H

#include "OCBase.h"

class OCFont : public OCFile // oft
{
public:
    class SingleFont
    {
    public:
        class Character
        {
        public:
            Character(QString ca = "", int lP = -1, int rP = -1, int hP = -1)
            {
                _character = ca;
                _leftPixel = lP;
                _rightPixel = rP;
                _highestPixelInFontRow = hP;
            }

            QString character() const { return _character; }
            void setCharacter(const QString &newCharacter) { _character = newCharacter; }

            int leftPixel() const { return _leftPixel; }
            void setLeftPixel(int newLeftPixel) { _leftPixel = newLeftPixel; }

            int rightPixel() const { return _rightPixel; }
            void setRightPixel(int newRightPixel) { _rightPixel = newRightPixel; }

            int highestPixelInFontRow() const { return _highestPixelInFontRow; }
            void setHighestPixelInFontRow(int newHighestPixelInFontRow) { _highestPixelInFontRow = newHighestPixelInFontRow; }

        private:
            QString _character;
            int _leftPixel;
            int _rightPixel;
            int _highestPixelInFontRow;
        };

        QList<Character> characters;

        QString name() const { return _name; }
        void setName(const QString &newName) { _name = newName; }
        QString colorTexture() const { return _colorTexture; }
        void setColorTexture(const QString &newColorTexture) { _colorTexture = newColorTexture; }
        QString alphaTexture() const { return _alphaTexture; }
        void setAlphaTexture(const QString &newAlphaTexture) { _alphaTexture = newAlphaTexture; }
        int maxHeightOfChars() const { return _maxHeightOfChars; }
        void setMaxHeightOfChars(const int &newMaxHeightOfChars) { _maxHeightOfChars = newMaxHeightOfChars; }
        int distanceBetweenChars() const { return _distanceBetweenChars; }
        void setDistanceBetweenChars(const int &newDistanceBetweenChars) { _distanceBetweenChars = newDistanceBetweenChars; }

    private:
        QString _name;
        QString _colorTexture;
        QString _alphaTexture;
        int _maxHeightOfChars;
        int _distanceBetweenChars;
    };

    QList<SingleFont> fonts;

    FileIOResponse read() override
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
                    fonts.append(SingleFont());
                    fonts.last().setName(in.readLine().toUtf8());
                    fonts.last().setColorTexture(in.readLine().toUtf8());
                    fonts.last().setAlphaTexture(in.readLine().toUtf8());
                    fonts.last().setMaxHeightOfChars(in.readLine().toUtf8().toInt());
                    fonts.last().setDistanceBetweenChars(in.readLine().toUtf8().toInt());
                }
                else if (line == "[char]")
                {
                    SingleFont::Character character;

                    character.setCharacter(in.readLine().toUtf8());
                    character.setLeftPixel(in.readLine().toUtf8().toInt());
                    character.setRightPixel(in.readLine().toUtf8().toInt());
                    character.setHighestPixelInFontRow(in.readLine().toUtf8().toInt());

                    if (!fonts.isEmpty()) fonts.last().characters.append(character);
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

    FileIOResponse write() override
    {
        QFile file(_path);

        if (!file.open(QFile::WriteOnly | QFile::Text)) { return FileIOResponse::errFileNotOpen; }

        QTextStream out(&file);

        out.setEncoding(QStringConverter::Latin1);

        try {
            OCBase base;
            out << base.writeFileHeader();
            out << "Author: " << set.read("main", "author").toString() << "\n";
            out << "Font count: " << fonts.count() << "\n";
            out << "Total characters: " << totalCharacterCount() << "\n";
            out << "\n";

            foreach (SingleFont font, fonts) {
                out << "#############################################" << "\n";
                out << "Font name: " << font.name() << "\n";
                out << "Characters: " << font.characters.count() << "\n";
                out << "\n";

                out << "[newfont]" << "\n";
                out << font.name() << "\n";
                out << font.colorTexture() << "\n";
                out << font.alphaTexture() << "\n";

                QString maxHeightOfChars = (font.maxHeightOfChars() == -1) ? "" : QString::number(font.maxHeightOfChars());
                out << maxHeightOfChars << "\n";
                QString distanceBetweenChars = (font.distanceBetweenChars() == -1) ? "" : QString::number(font.distanceBetweenChars());
                out << distanceBetweenChars << "\n";
                out << "\n";

                foreach (SingleFont::Character character, font.characters)
                {
                    out << "[char]" << "\n";
                    out << character.character() << "\n";
                    out << character.leftPixel() << "\n";
                    out << character.rightPixel() << "\n";
                    out << character.highestPixelInFontRow() << "\n";
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

    virtual void clear() override { fonts.clear(); _path.clear(); _encoding = QStringConverter::Latin1; }

    int totalCharacterCount()
    {
        int characterCount = 0;
        foreach (OCFont::SingleFont font, fonts) characterCount += font.characters.count();
        return characterCount;
    }

    inline QString path() const { return _path; }
    inline void setPath(const QString &newPath) { _path = newPath; }

    inline QStringConverter::Encoding encoding() const { return _encoding; }
    inline void setEncoding(QStringConverter::Encoding newEncoding) { _encoding = newEncoding; }

    enum Selection
    {
        Font,
        Character
    };

    QMap<Selection, int> selection;

private:
    QString _path;
    QStringConverter::Encoding _encoding;
    OTSettings set;
};

#endif // OCFONT_H
