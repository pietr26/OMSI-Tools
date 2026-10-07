#include "OTFolderTransfer.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>

#include "OTPath.h"

namespace
{
    /// Like OTPath::resolve(), but keeps the real spelling of every directory that already
    /// exists even if the rest of the path doesn't - so on Linux nothing gets created a
    /// second time in a different case.
    QString targetPath(const QString &base, const QString &relativePath)
    {
        QString path = base;
        for (const QString &segment : relativePath.split('/', Qt::SkipEmptyParts))
        {
            const QString resolved = OTPath::resolve(path, segment);
            path = QFileInfo::exists(resolved) ? resolved : path + "/" + segment;
        }

        return path;
    }
}

OTFolderTransfer::Result OTFolderTransfer::transfer(const QString &sourceRoot, const QString &targetRoot, const QString &relativeFolder, Mode mode)
{
    Result result;

    const QString sourceFolder = OTPath::resolve(sourceRoot, OTPath::normalize(relativeFolder));
    if (!QFileInfo(sourceFolder).isDir())
        return result;

    // The source's real spelling, so a folder which is new in the target is named like there.
    const QString folder = QDir(sourceRoot).relativeFilePath(sourceFolder);
    const QString targetFolder = targetPath(targetRoot, folder);
    const QDir source(sourceFolder);
    const QDir::Filters includeHidden = QDir::Hidden | QDir::System;

    QStringList directories;
    QDirIterator listDirectories(sourceFolder, QDir::Dirs | QDir::NoDotAndDotDot | includeHidden, QDirIterator::Subdirectories);
    while (listDirectories.hasNext()) directories << listDirectories.next();

    // Collect first - the directory listing isn't reliable while its entries are moved away.
    QStringList files;
    QDirIterator listFiles(sourceFolder, QDir::Files | includeHidden, QDirIterator::Subdirectories);
    while (listFiles.hasNext()) files << source.relativeFilePath(listFiles.next());

    QDir().mkpath(targetFolder);
    for (const QString &directory : std::as_const(directories))
        QDir().mkpath(targetPath(targetFolder, source.relativeFilePath(directory)));

    for (const QString &file : std::as_const(files))
    {
        QCoreApplication::processEvents();

        const QString name = folder + "/" + file;
        const QString target = targetPath(targetFolder, file);

        if (QFileInfo::exists(target))
            result.skipped << name;
        else if (mode == Move ? QDir().rename(source.filePath(file), target) : QFile::copy(source.filePath(file), target))
            result.transferred << name;
        else
        {
            qWarning().noquote() << QString("Could not %1 '%2' to '%3'.").arg(mode == Move ? "move" : "copy", source.filePath(file), target);
            result.failed << name;
        }
    }

    if (mode == Move)
    {
        // Deepest directories first - rmdir() refuses every directory which isn't empty.
        std::sort(directories.begin(), directories.end(), [](const QString &a, const QString &b) { return a.size() > b.size(); });
        for (const QString &directory : std::as_const(directories)) QDir().rmdir(directory);
        QDir().rmdir(sourceFolder);
    }

    return result;
}
