#ifndef OTDDSIMAGE_H
#define OTDDSIMAGE_H

#include <QImage>
#include <QString>

/*!
    Minimal reader for DirectDraw Surfaces (DDS).

    OMSI stores nearly all of its textures as DDS. On Windows they are handed over to
    DirectX' texconv.exe, but that tool exists nowhere else - and Qt does not help out
    either: the DDS image plugin of Qt 5 was dropped in Qt 6 and is not part of the Qt
    Image Formats module any more. So the surface types OMSI actually uses are decoded
    here: DXT1 - DXT5 and the uncompressed layouts which describe themselves through
    their channel masks.

    Only the first mipmap level of the first surface is read, which is what all callers
    display respectively evaluate.
*/
namespace OTDdsImage
{
    /// Returns a null image if the file is no DDS or uses an unsupported surface format.
    QImage read(const QString &fileName);
}

#endif // OTDDSIMAGE_H
