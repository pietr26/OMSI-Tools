#include "OCFont.h"
#include "OTBackend/OTSettings.h"
#include <QFile>
#include <QString>
#include <QStringConverter>
#include <QTextStream>

OCBase::File::FileIOResponse OCFont::FontCollection::read()
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

OCBase::File::FileIOResponse OCFont::FontCollection::write()
{
    QFile file(path);

    // Without QFile::Text, so the terminator below is what ends up in the file on every
    // platform - the flag only produced CRLF on Windows, and OMSI writes CRLF.
    if (!file.open(QFile::WriteOnly)) { return FileIOResponse::errFileNotOpen; }

    QTextStream out(&file);

    out.setEncoding(QStringConverter::Latin1);

    const QString nl = "\r\n";

    // The file header carries its own line breaks.
    const auto multiline = [&nl](QString text) { return text.replace("\n", nl); };

    try {
        OTSettings set;

        out << multiline(OCBase::writeFileHeader());
        out << "Author: " << set.read("main", "author").toString() << nl;
        out << "Font count: " << fonts.count() << nl;
        out << "Total characters: " << totalCharacterCount() << nl;
        out << nl;

        foreach (SingleFont *font, fonts) {
            out << "#############################################" << nl;
            out << "Font name: " << font->name << nl;
            out << "Characters: " << font->characters.count() << nl;
            out << nl;

            out << "[newfont]" << nl;
            out << font->name << nl;
            out << font->colorTexture << nl;
            out << font->alphaTexture << nl;

            QString maxHeightOfChars = (font->maxHeightOfChars == -1) ? "" : QString::number(font->maxHeightOfChars);
            out << maxHeightOfChars << nl;
            QString distanceBetweenChars = (font->distanceBetweenChars == -1) ? "" : QString::number(font->distanceBetweenChars);
            out << distanceBetweenChars << nl;
            out << nl;

            foreach (Character *character, font->characters)
            {
                out << "[char]" << nl;
                out << character->character << nl;
                out << character->leftPixel << nl;
                out << character->rightPixel << nl;
                out << character->highestPixelInFontRow << nl;
                out << nl;
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

void OCFont::FontCollection::clear() { fonts.clear(); path.clear(); encoding = QStringConverter::Latin1; }

int OCFont::FontCollection::totalCharacterCount()
{
    int characterCount = 0;
    foreach (SingleFont* font, fonts) characterCount += font->characters.count();
    return characterCount;
}
