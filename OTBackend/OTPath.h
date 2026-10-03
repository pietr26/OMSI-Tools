#ifndef OTPATH_H
#define OTPATH_H

#include <QString>

/*!
    Helpers for paths which come out of OMSI's own configuration files.

    OMSI writes its relative paths in Windows notation ("Sceneryobjects\Berlin\x.sco")
    and relies on the case-insensitive Windows file system. Both assumptions break on
    Linux, therefore every file system access which is built from such a path has to be
    routed through OTPath::resolve().

    On Windows all functions are a plain concatenation, so the behaviour there is
    unchanged.
*/
namespace OTPath
{
    /// Turns OMSI's backslash notation into the notation Qt expects.
    QString normalize(const QString &path);

    /// Joins an OMSI relative path to a base directory and returns a path the local file
    /// system accepts. On case-sensitive file systems the single path segments are
    /// additionally matched case-insensitively. If nothing matches, the plain
    /// concatenation is returned so callers can still report the file as missing.
    QString resolve(const QString &basePath, const QString &relativePath);

    /// Convenience for the very common "does this OMSI file exist?" check.
    bool exists(const QString &basePath, const QString &relativePath);

    /// Drops the cached directory listings used by resolve(). Has to be called after the
    /// application itself moved, renamed or deleted files inside the OMSI directory.
    void clearCache();
}

#endif // OTPATH_H
