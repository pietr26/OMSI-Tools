#include "OTFileSource.h"

OTFileSource::OTFileSource(const QString &fileName) :
    _fileName(fileName) {
}

QString OTFileSource::fileName() const {
    return _fileName;
}

QStringList OTFileSource::sources() const {
    return _sources;
}

void OTFileSource::addSource(const QString &newSource) {
    _sources << newSource;
}

void OTFileSource::addSources(const QStringList &newSources) {
    for(QString source : newSources)
        _sources << source;
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
