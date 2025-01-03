#include "OTModelConfigValidator.h"

#include <QTextStream>
#include <QFile>

OTModelConfigValidator::OTModelConfigValidator(QObject *parent, const QString &filePath) :
    OTContentValidator(parent, filePath) {}

void OTModelConfigValidator::specificValidate() {
    while(!_stream->atEnd()) {
        readNextLine();
        validateLine();
    }
}

void OTModelConfigValidator::validateLine() {
    // missing mesh files
    // FIXME: remove spaces
    if(_currentLine == "[mesh]" || _currentLine == "[collision_mesh]") {
        _meshFound = true;
        _matlFound = false;
        QString mesh = readNextLine();
        if(!QFile::exists(_fileDir + "/model/" + mesh)) {
            throwIssue(OTContentValidatorIssue::MissingMeshFile, {mesh});
            return;
        } else {
            addLinkedFile(_fileDir + "/model/" + mesh);
        }
    }

    // materials
    if(_currentLine == "[matl]" || _currentLine == "[matl_change]") {
        _matlFound = true;
        if(!_meshFound) {
            throwIssue(OTContentValidatorIssue::MaterialWithoutMesh, {});
            return;
        }
    }

    if(_matlProperties.contains(_currentLine) && !_matlFound) {
        throwIssue(OTContentValidatorIssue::MaterialPropertyWithoutMaterial, {});
        return;
    }

    // missing script files
    if(_currentLine == "[varnamelist]" || _currentLine == "[stringvarnamelist]" || _currentLine == "[script]" || _currentLine == "[constfile]") {
        QString countStr = readNextLine();
        bool ok;
        int count = countStr.toInt(&ok);
        if(!ok) {
            throwIssue(OTContentValidatorIssue::InvalidIntegerValue, {countStr});
            return;
        }

        readNextLine();
        int i = 1;
        while(i <= count && !_stream->atEnd()) {
            if(!QFile::exists(_fileDir + "/" + _currentLine)) {
                throwIssue(OTContentValidatorIssue::MissingScriptFile, {_currentLine});
            } else {
                addLinkedFile(_fileDir + "/" + _currentLine);
            }
            i++;
            readNextLine();
        }
    }
}
