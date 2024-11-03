#include "OCFont.h"

OCBase::File::FileIOResponse FontCollection::read()
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        clear();
        return FileIOResponse::errFileNotOpen;
    }

    QTextStream in(&file);
    in.setEncoding(encoding);
    qDebug().noquote() << "Font encoding:" << encoding;
    QString line;

    try
    {
        while (!in.atEnd())
        {
            line = in.readLine();

            if (line == "[newfont]")
            {
                fonts.append(new SingleFont());
                fonts.last()->name = in.readLine().toUtf8();
                fonts.last()->colorTexture = in.readLine().toUtf8();
                fonts.last()->alphaTexture = in.readLine().toUtf8();
                fonts.last()->maxHeightOfChars = in.readLine().toUtf8().toInt();
                fonts.last()->distanceBetweenChars = in.readLine().toUtf8().toInt();
            }
            else if (line == "[char]")
            {
                Character *character = new Character();

                character->character = in.readLine().toUtf8();
                character->leftPixel = in.readLine().toUtf8().toInt();
                character->rightPixel = in.readLine().toUtf8().toInt();
                character->highestPixelInFontRow = in.readLine().toUtf8().toInt();

                if (!fonts.isEmpty()) fonts.last()->characters.append(character);
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
    QFile file(path);

    if (!file.open(QFile::WriteOnly | QFile::Text)) { return FileIOResponse::errFileNotOpen; }

    QTextStream out(&file);

    out.setEncoding(QStringConverter::Latin1);

    try {
        OTSettings set;

        out << OCBase::writeFileHeader();
        out << "Author: " << set.read("main", "author").toString() << "\n";
        out << "Font count: " << fonts.count() << "\n";
        out << "Total characters: " << totalCharacterCount() << "\n";
        out << "\n";

        foreach (SingleFont *font, fonts) {
            out << "#############################################" << "\n";
            out << "Font name: " << font->name << "\n";
            out << "Characters: " << font->characters.count() << "\n";
            out << "\n";

            out << "[newfont]" << "\n";
            out << font->name << "\n";
            out << font->colorTexture << "\n";
            out << font->alphaTexture << "\n";

            QString maxHeightOfChars = (font->maxHeightOfChars == -1) ? "" : QString::number(font->maxHeightOfChars);
            out << maxHeightOfChars << "\n";
            QString distanceBetweenChars = (font->distanceBetweenChars == -1) ? "" : QString::number(font->distanceBetweenChars);
            out << distanceBetweenChars << "\n";
            out << "\n";

            foreach (Character *character, font->characters)
            {
                out << "[char]" << "\n";
                out << character->character << "\n";
                out << character->leftPixel << "\n";
                out << character->rightPixel << "\n";
                out << character->highestPixelInFontRow << "\n";
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

void FontCollection::clear() { fonts.clear(); path.clear(); encoding = QStringConverter::Latin1; }

int FontCollection::totalCharacterCount()
{
    int characterCount = 0;
    foreach (SingleFont* font, fonts) characterCount += font->characters.count();
    return characterCount;
}
