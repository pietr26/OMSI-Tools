#include "OTFileSource.h"

OTFileSource::OTFileSource(const QString &fileName, FileType fileType) :
    _fileName(fileName), _fileType(fileType) {
}

QString OTFileSource::fileName() const {
    return _fileName;
}

OTFileSource::FileType OTFileSource::fileType() const {
    return _fileType;
}

QList<QPair<QString, int>> OTFileSource::sources() {
    return _sources;
}

QStringList OTFileSource::sourceStrings() {
    QStringList result;
    for(QPair<QString, int> current : _sources)
        result << current.first;

    return result;
}

int OTFileSource::sourcesCount() {
    return sources().count();
}

void OTFileSource::addOccurrence(const QString &newSource) {
    for(QPair<QString, int> &current : _sources) {
        if(current.first == newSource) {
            current.second++;
            return;
        }
    }

    _sources << QPair<QString, int>(newSource, 1);
}

int OTFileSource::occurrencesCount() const {
    int count = 0;
    for(QPair<QString, int> current : _sources) {
        count += current.second;
    }
    return count;
}

QString OTFileSource::errorString() const {
    QStringList errors;
    if(!_missingMeshs.empty())
        errors << QObject::tr("%n missing mesh file(s)", "", _missingMeshs.count());

    if(!_missingScriptFiles.empty())
        errors << QObject::tr("%n missing script file(s)", "", _missingScriptFiles.count());

    if(!_missingTextures.empty())
        errors << QObject::tr("%n missing texture(s)", "", _missingTextures.count());

    return errors.join("\n");
}

bool OTFileSource::hasMissingMesh() const {
    return !_missingMeshs.isEmpty();
}

QStringList OTFileSource::missingMeshs() const {
    return _missingMeshs;
}

void OTFileSource::addMissingMesh(const QString &newMesh) {
    _missingMeshs << newMesh;
}

bool OTFileSource::hasMissingScriptFile() const {
    return !_missingScriptFiles.isEmpty();
}

QStringList OTFileSource::missingScriptFiles() const {
    return _missingScriptFiles;
}

void OTFileSource::addMissingScriptFile(const QString &newScriptFile) {
    _missingScriptFiles << newScriptFile;
}

bool OTFileSource::hasMissingTexture() {
    return !_missingTextures.isEmpty();
}

QStringList OTFileSource::missingTextures() const {
    return _missingTextures;
}

void OTFileSource::addMissingTexture(const QString &newTexture) {
    _missingTextures << newTexture;
}

