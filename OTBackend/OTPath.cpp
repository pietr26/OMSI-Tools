#include "OTPath.h"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <QMutex>
#include <QMutexLocker>

QString OTPath::normalize(const QString &path)
{
    QString result = path;
    return result.replace('\\', '/');
}

namespace
{
    /// Some callers hand over directories with a trailing separator, which would end up
    /// as a doubled one in the middle of the result.
    QString trimmedBase(const QString &basePath)
    {
        QString result = OTPath::normalize(basePath);

        while ((result.length() > 1) && result.endsWith('/'))
            result.chop(1);

        return result;
    }
}

#ifdef Q_OS_WIN

// The Windows file system resolves the case on its own and accepts both separators.
QString OTPath::resolve(const QString &basePath, const QString &relativePath)
{
    if (basePath.isEmpty()) return relativePath;
    if (relativePath.isEmpty()) return basePath;

    return trimmedBase(basePath) + "/" + relativePath;
}

void OTPath::clearCache() { }

#else

namespace
{
    struct DirectoryIndex
    {
        QDateTime lastModified;

        /// Lower cased entry name -> real entry name.
        QHash<QString, QString> entries;
    };

    QHash<QString, DirectoryIndex> caseIndex;
    QMutex caseIndexMutex;

    /// Returns the real name of 'entry' inside 'dirPath', or an empty string if the
    /// directory doesn't contain anything matching it.
    QString realName(const QString &dirPath, const QString &entry)
    {
        QMutexLocker locker(&caseIndexMutex);

        // The modification time of a directory changes as soon as entries are added,
        // renamed or removed - so a cached listing invalidates itself.
        const QDateTime lastModified = QFileInfo(dirPath).lastModified();

        if (!caseIndex.contains(dirPath) || (caseIndex.value(dirPath).lastModified != lastModified))
        {
            DirectoryIndex index;
            index.lastModified = lastModified;

            for (const QString &current : QDir(dirPath).entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System))
                index.entries.insert(current.toLower(), current);

            caseIndex.insert(dirPath, index);
        }

        return caseIndex.value(dirPath).entries.value(entry.toLower());
    }
}

QString OTPath::resolve(const QString &basePath, const QString &relativePath)
{
    if (basePath.isEmpty()) return normalize(relativePath);
    if (relativePath.isEmpty()) return basePath;

    const QString base = trimmedBase(basePath);
    const QString normalized = normalize(relativePath);
    const QString plain = base + "/" + normalized;

    // Fast path: content with correct spelling doesn't need any directory listing.
    if (QFileInfo::exists(plain)) return plain;

    QString resolved = base;

    for (const QString &segment : normalized.split('/', Qt::SkipEmptyParts))
    {
        if (QFileInfo::exists(resolved + "/" + segment))
        {
            resolved += "/" + segment;
            continue;
        }

        const QString match = realName(resolved, segment);
        if (match.isEmpty())
        {
            // Nothing matches - hand back the untouched path so the caller reports the
            // remaining segments as missing, exactly like on Windows.
            return plain;
        }

        resolved += "/" + match;
    }

    return resolved;
}

void OTPath::clearCache()
{
    QMutexLocker locker(&caseIndexMutex);
    caseIndex.clear();
}

#endif

bool OTPath::exists(const QString &basePath, const QString &relativePath)
{
    return QFileInfo::exists(resolve(basePath, relativePath));
}
