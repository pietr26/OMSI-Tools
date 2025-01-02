#include "OTFileSource.h"

OTFileSource::OTFileSource(const QString &fileName, FileType fileType) :
    _fileName(fileName), _fileType(fileType) {
}

bool OTFileSource::operator==(const OTFileSource &other) {
    return fileName() == other.fileName();
}

bool OTFileSource::operator<(const OTFileSource &other) {
    return fileName() < other.fileName();
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

int OTFileSource::occurrencesCount() const {
    int count = 0;
    for(QPair<QString, int> current : _sources) {
        count += current.second;
    }
    return count;
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

OTContentValidatorResult OTFileSource::advancedCheckResult() const {
    return _advancedCheckResult;
}

void OTFileSource::setAdvancedCheckResult(const OTContentValidatorResult &result) {
    _advancedCheckResult = result;
}
