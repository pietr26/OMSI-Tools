#include "OTDdsImage.h"

#include <QDataStream>
#include <QDebug>
#include <QFile>

namespace
{
    // DDS_PIXELFORMAT.dwFlags
    const quint32 formatAlphaPixels = 0x00000001;
    const quint32 formatAlpha       = 0x00000002;
    const quint32 formatFourCC      = 0x00000004;
    const quint32 formatRGB         = 0x00000040;
    const quint32 formatLuminance   = 0x00020000;

    // DXGI_FORMAT values of the "DX10" header extension
    const quint32 dxgiRGBA8         = 28;
    const quint32 dxgiRGBA8SRGB     = 29;
    const quint32 dxgiBC1           = 71;
    const quint32 dxgiBC1SRGB       = 72;
    const quint32 dxgiBC2           = 74;
    const quint32 dxgiBC2SRGB       = 75;
    const quint32 dxgiBC3           = 77;
    const quint32 dxgiBC3SRGB       = 78;
    const quint32 dxgiBGRA8         = 87;
    const quint32 dxgiBGRX8         = 88;
    const quint32 dxgiBGRA8SRGB     = 91;
    const quint32 dxgiBGRX8SRGB     = 93;

    constexpr quint32 tag(char a, char b, char c, char d)
    {
        return quint32(a) | (quint32(b) << 8) | (quint32(c) << 16) | (quint32(d) << 24);
    }

    struct PixelFormat
    {
        quint32 flags = 0;
        quint32 fourCC = 0;
        quint32 bitCount = 0;
        quint32 redMask = 0;
        quint32 greenMask = 0;
        quint32 blueMask = 0;
        quint32 alphaMask = 0;
    };

    struct Header
    {
        quint32 width = 0;
        quint32 height = 0;
        PixelFormat pixelFormat;
        /// Only filled if the pixel format is "DX10".
        quint32 dxgiFormat = 0;
    };

    struct Color
    {
        int r = 0;
        int g = 0;
        int b = 0;
        int a = 255;
    };

    /// One channel of an uncompressed surface, described by its mask.
    class Channel
    {
    public:
        explicit Channel(quint32 mask) : _mask(mask)
        {
            if (_mask == 0) return;

            while (!((_mask >> _shift) & 1)) _shift++;

            quint32 rest = _mask >> _shift;
            int bits = 0;
            while (rest & 1) { bits++; rest >>= 1; }

            _max = (bits >= 32) ? 0xFFFFFFFF : ((quint32(1) << bits) - 1);
        }

        bool isEmpty() const { return _mask == 0; }

        int valueOf(quint32 pixel, int fallback) const
        {
            if (_mask == 0) return fallback;

            const quint32 raw = (pixel & _mask) >> _shift;

            // Spread the channel over the full 8 bit range - 5 bit 31 has to become 255,
            // not 31. Rounding here is what makes it identical to a bit replication.
            return (_max == 255) ? int(raw) : int((raw * 255 + _max / 2) / _max);
        }

    private:
        quint32 _mask = 0;
        int _shift = 0;
        quint32 _max = 0;
    };

    void expand565(quint16 value, Color &color)
    {
        color.r = ((value & 0xF800) >> 8); color.r |= color.r >> 5;
        color.g = ((value & 0x07E0) >> 3); color.g |= color.g >> 6;
        color.b = ((value & 0x001F) << 3); color.b |= color.b >> 5;
        color.a = 255;
    }

    /// The colour part of a DXT block - identical for DXT1 to DXT5, except that only
    /// DXT1 uses the second mode with a transparent fourth colour.
    void decodeColorBlock(const uchar *block, Color colors[16], bool allowTransparency)
    {
        const quint16 value0 = quint16(block[0]) | (quint16(block[1]) << 8);
        const quint16 value1 = quint16(block[2]) | (quint16(block[3]) << 8);

        Color palette[4];
        expand565(value0, palette[0]);
        expand565(value1, palette[1]);

        if ((value0 > value1) || !allowTransparency)
        {
            palette[2].r = (2 * palette[0].r + palette[1].r) / 3;
            palette[2].g = (2 * palette[0].g + palette[1].g) / 3;
            palette[2].b = (2 * palette[0].b + palette[1].b) / 3;

            palette[3].r = (2 * palette[1].r + palette[0].r) / 3;
            palette[3].g = (2 * palette[1].g + palette[0].g) / 3;
            palette[3].b = (2 * palette[1].b + palette[0].b) / 3;
        }
        else
        {
            palette[2].r = (palette[0].r + palette[1].r) / 2;
            palette[2].g = (palette[0].g + palette[1].g) / 2;
            palette[2].b = (palette[0].b + palette[1].b) / 2;

            palette[3] = Color{0, 0, 0, 0};
        }

        quint32 indices = quint32(block[4]) | (quint32(block[5]) << 8) | (quint32(block[6]) << 16) | (quint32(block[7]) << 24);

        for (int pixel = 0; pixel < 16; pixel++)
            colors[pixel] = palette[(indices >> (2 * pixel)) & 3];
    }

    /// The four bit alpha channel of a DXT2 / DXT3 block.
    void decodeAlphaBlockExplicit(const uchar *block, Color colors[16])
    {
        for (int pixel = 0; pixel < 16; pixel++)
        {
            const uchar pair = block[pixel / 2];
            colors[pixel].a = ((pixel % 2) ? (pair >> 4) : (pair & 0x0F)) * 17;
        }
    }

    /// The interpolated alpha channel of a DXT4 / DXT5 block.
    void decodeAlphaBlockInterpolated(const uchar *block, Color colors[16])
    {
        int alpha[8];
        alpha[0] = block[0];
        alpha[1] = block[1];

        if (alpha[0] > alpha[1])
            for (int step = 0; step < 6; step++)
                alpha[2 + step] = ((6 - step) * alpha[0] + (1 + step) * alpha[1]) / 7;
        else
        {
            for (int step = 0; step < 4; step++)
                alpha[2 + step] = ((4 - step) * alpha[0] + (1 + step) * alpha[1]) / 5;

            alpha[6] = 0;
            alpha[7] = 255;
        }

        quint64 indices = 0;
        for (int byte = 0; byte < 6; byte++)
            indices |= quint64(block[2 + byte]) << (8 * byte);

        for (int pixel = 0; pixel < 16; pixel++)
            colors[pixel].a = alpha[(indices >> (3 * pixel)) & 7];
    }

    void writeBlock(QImage &image, const Color colors[16], int blockX, int blockY)
    {
        for (int pixel = 0; pixel < 16; pixel++)
        {
            const int x = blockX + (pixel % 4);
            const int y = blockY + (pixel / 4);

            // The last block of a row respectively column may stick out of the surface.
            if ((x >= image.width()) || (y >= image.height())) continue;

            const Color &color = colors[pixel];
            reinterpret_cast<QRgb *>(image.scanLine(y))[x] = qRgba(color.r, color.g, color.b, color.a);
        }
    }

    QImage decodeCompressed(const QByteArray &data, int width, int height, int blockSize, bool explicitAlpha, bool interpolatedAlpha)
    {
        const int blocksPerRow = (width + 3) / 4;
        const int blocksPerColumn = (height + 3) / 4;

        if (data.size() < (qsizetype(blocksPerRow) * blocksPerColumn * blockSize))
        {
            qCritical() << "DDS: surface data is truncated.";
            return QImage();
        }

        QImage image(width, height, QImage::Format_ARGB32);
        const uchar *source = reinterpret_cast<const uchar *>(data.constData());

        for (int blockY = 0; blockY < blocksPerColumn; blockY++)
        {
            for (int blockX = 0; blockX < blocksPerRow; blockX++)
            {
                const uchar *block = source + (qsizetype(blockY) * blocksPerRow + blockX) * blockSize;
                Color colors[16];

                // With DXT1 the second colour mode means transparency, with every other
                // variant the alpha comes out of its own block and the mode is unused.
                decodeColorBlock(block + (blockSize - 8), colors, blockSize == 8);

                if (explicitAlpha) decodeAlphaBlockExplicit(block, colors);
                else if (interpolatedAlpha) decodeAlphaBlockInterpolated(block, colors);

                writeBlock(image, colors, blockX * 4, blockY * 4);
            }
        }

        return image;
    }

    QImage decodeUncompressed(const QByteArray &data, int width, int height, const PixelFormat &pixelFormat)
    {
        const int bytesPerPixel = int(pixelFormat.bitCount) / 8;

        if ((pixelFormat.bitCount % 8) || (bytesPerPixel < 1) || (bytesPerPixel > 4))
        {
            qCritical() << "DDS: unsupported colour depth" << pixelFormat.bitCount;
            return QImage();
        }

        if (data.size() < (qsizetype(width) * height * bytesPerPixel))
        {
            qCritical() << "DDS: surface data is truncated.";
            return QImage();
        }

        const bool isLuminance = (pixelFormat.flags & formatLuminance);
        const bool isAlphaOnly = (pixelFormat.flags & formatAlpha) && !(pixelFormat.flags & (formatRGB | formatLuminance));

        const Channel red(pixelFormat.redMask);
        const Channel green(pixelFormat.greenMask);
        const Channel blue(pixelFormat.blueMask);
        const Channel alpha((pixelFormat.flags & (formatAlphaPixels | formatAlpha)) ? pixelFormat.alphaMask : 0);

        QImage image(width, height, QImage::Format_ARGB32);
        const uchar *source = reinterpret_cast<const uchar *>(data.constData());

        for (int y = 0; y < height; y++)
        {
            QRgb *target = reinterpret_cast<QRgb *>(image.scanLine(y));

            for (int x = 0; x < width; x++)
            {
                quint32 pixel = 0;
                for (int byte = 0; byte < bytesPerPixel; byte++)
                    pixel |= quint32(source[(qsizetype(y) * width + x) * bytesPerPixel + byte]) << (8 * byte);

                // A pure alpha surface has no colour at all, a luminance one repeats its
                // single channel - everything else reads its channels from the masks.
                if (isAlphaOnly)
                    target[x] = qRgba(0, 0, 0, alpha.valueOf(pixel, 255));
                else if (isLuminance)
                {
                    const int grey = red.valueOf(pixel, 0);
                    target[x] = qRgba(grey, grey, grey, alpha.valueOf(pixel, 255));
                }
                else
                    target[x] = qRgba(red.valueOf(pixel, 0), green.valueOf(pixel, 0), blue.valueOf(pixel, 0), alpha.valueOf(pixel, 255));
            }
        }

        return image;
    }

    /// Fills in the pixel format a "DX10" header describes, so that the decoding itself
    /// does not have to know about the extension any more.
    bool applyDxgiFormat(Header &header)
    {
        switch (header.dxgiFormat)
        {
        case dxgiBC1: case dxgiBC1SRGB: header.pixelFormat.fourCC = tag('D', 'X', 'T', '1'); return true;
        case dxgiBC2: case dxgiBC2SRGB: header.pixelFormat.fourCC = tag('D', 'X', 'T', '3'); return true;
        case dxgiBC3: case dxgiBC3SRGB: header.pixelFormat.fourCC = tag('D', 'X', 'T', '5'); return true;

        case dxgiRGBA8: case dxgiRGBA8SRGB:
            header.pixelFormat.flags = formatRGB | formatAlphaPixels;
            header.pixelFormat.fourCC = 0;
            header.pixelFormat.bitCount = 32;
            header.pixelFormat.redMask   = 0x000000FF;
            header.pixelFormat.greenMask = 0x0000FF00;
            header.pixelFormat.blueMask  = 0x00FF0000;
            header.pixelFormat.alphaMask = 0xFF000000;
            return true;

        case dxgiBGRA8: case dxgiBGRA8SRGB: case dxgiBGRX8: case dxgiBGRX8SRGB:
            header.pixelFormat.flags = formatRGB | (((header.dxgiFormat == dxgiBGRA8) || (header.dxgiFormat == dxgiBGRA8SRGB)) ? formatAlphaPixels : 0);
            header.pixelFormat.fourCC = 0;
            header.pixelFormat.bitCount = 32;
            header.pixelFormat.redMask   = 0x00FF0000;
            header.pixelFormat.greenMask = 0x0000FF00;
            header.pixelFormat.blueMask  = 0x000000FF;
            header.pixelFormat.alphaMask = 0xFF000000;
            return true;

        default:
            qCritical() << "DDS: unsupported DXGI format" << header.dxgiFormat;
            return false;
        }
    }
}

QImage OTDdsImage::read(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly))
    {
        qCritical().noquote() << "DDS: could not open '" + fileName + "'.";
        return QImage();
    }

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    quint32 magic = 0;
    quint32 headerSize = 0;
    in >> magic >> headerSize;

    if ((magic != tag('D', 'D', 'S', ' ')) || (headerSize != 124))
    {
        qCritical().noquote() << "DDS: '" + fileName + "' is no DirectDraw Surface.";
        return QImage();
    }

    Header header;
    quint32 skip = 0;

    in >> skip;                             // dwFlags
    in >> header.height >> header.width;
    in >> skip >> skip >> skip;             // dwPitchOrLinearSize, dwDepth, dwMipMapCount
    for (int reserved = 0; reserved < 11; reserved++) in >> skip;

    in >> skip;                             // ddspf.dwSize
    in >> header.pixelFormat.flags >> header.pixelFormat.fourCC >> header.pixelFormat.bitCount;
    in >> header.pixelFormat.redMask >> header.pixelFormat.greenMask >> header.pixelFormat.blueMask >> header.pixelFormat.alphaMask;

    for (int trailing = 0; trailing < 5; trailing++) in >> skip;    // dwCaps 1 - 4, dwReserved2

    if ((header.pixelFormat.flags & formatFourCC) && (header.pixelFormat.fourCC == tag('D', 'X', '1', '0')))
    {
        in >> header.dxgiFormat;
        for (int trailing = 0; trailing < 4; trailing++) in >> skip;

        if (!applyDxgiFormat(header)) return QImage();
    }

    if ((in.status() != QDataStream::Ok) || (header.width == 0) || (header.height == 0))
    {
        qCritical().noquote() << "DDS: '" + fileName + "' has a damaged header.";
        return QImage();
    }

    // Everything behind the header starts with the first mipmap level of the first
    // surface, and that is the only one anybody here is interested in.
    const QByteArray data = file.readAll();
    const int width = int(header.width);
    const int height = int(header.height);

    QImage image;

    if (header.pixelFormat.flags & formatFourCC)
    {
        switch (header.pixelFormat.fourCC)
        {
        case tag('D', 'X', 'T', '1'): image = decodeCompressed(data, width, height,  8, false, false); break;
        case tag('D', 'X', 'T', '2'):
        case tag('D', 'X', 'T', '3'): image = decodeCompressed(data, width, height, 16, true,  false); break;
        case tag('D', 'X', 'T', '4'):
        case tag('D', 'X', 'T', '5'): image = decodeCompressed(data, width, height, 16, false, true);  break;

        default:
        {
            const QString name = QString::fromLatin1(reinterpret_cast<const char *>(&header.pixelFormat.fourCC), 4);
            qCritical().noquote() << "DDS: '" + fileName + "' uses the unsupported surface format '" + name + "'.";
            return QImage();
        }
        }
    }
    else
        image = decodeUncompressed(data, width, height, header.pixelFormat);

    // DXT2 and DXT4 store their colours multiplied with the alpha value.
    if ((header.pixelFormat.fourCC == tag('D', 'X', 'T', '2')) || (header.pixelFormat.fourCC == tag('D', 'X', 'T', '4')))
    {
        for (int y = 0; y < image.height(); y++)
        {
            QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));

            for (int x = 0; x < image.width(); x++)
            {
                const int alpha = qAlpha(line[x]);
                if (alpha == 0) continue;

                line[x] = qRgba(qMin(255, qRed(line[x])   * 255 / alpha),
                                qMin(255, qGreen(line[x]) * 255 / alpha),
                                qMin(255, qBlue(line[x])  * 255 / alpha), alpha);
            }
        }
    }

    return image;
}
