#ifndef OTFOLDERTRANSFER_H
#define OTFOLDERTRANSFER_H

#include <QStringList>

/*!
    Copies or moves a folder file by file. Existing files are never overwritten, and after
    moving only the directories which ran empty are removed - so a file which couldn't be
    transferred always stays where it was.
*/
namespace OTFolderTransfer
{
    enum Mode { Copy, Move };

    /// All paths relative to the roots, e.g. "Sceneryobjects/Hamburg/model/a.o3d".
    struct Result
    {
        QStringList transferred;
        QStringList skipped; ///< already present in the target
        QStringList failed;
    };

    /// Transfers sourceRoot/relativeFolder into targetRoot/relativeFolder. relativeFolder may
    /// be in OMSI's backslash notation; existing directories in the target are reused even if
    /// their case differs.
    Result transfer(const QString &sourceRoot, const QString &targetRoot, const QString &relativeFolder, Mode mode);
}

#endif // OTFOLDERTRANSFER_H
