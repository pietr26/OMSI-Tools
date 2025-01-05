#include "OTSplineValidator.h"

#include "OTBackend/OTOMSIFileHandler.h"

#include <QFile>
#include <QTextStream>

OTSplineValidator::OTSplineValidator(QObject *parent, const QString &filePath) :
    OTModelConfigValidator(parent, filePath) {
    _splineScanning = true;
}

void OTSplineValidator::specificValidate() {
    while(!_stream->atEnd()) {
        readNextLine();
        validateLine();
        OTModelConfigValidator::validateLine();
    }

    finalizeValidation();
    OTModelConfigValidator::finalizeValidation();
}

void OTSplineValidator::validateLine() {
    // heightprofile definition
    if(_currentLine == "[heightprofile]") {
        _splineTextureFound = false;
        for(int i = 0; i < 4; i++)
            if(!isValidFloat(readNextLine()))
                throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});
        return;
    }

    // texture definition
    if(_currentLine == "[texture]") {
        _splineTextureFound = true;
        _meshFound = true;
        readMaterial(true);

        _textureCount++;
        return;
    }

    // TODO: Validate PatchworkChain

    // profile definition
    if(_currentLine == "[profile]") {
        checkLastProfilePointCount();
        _splineTextureFound = false;

        _lastProfileStart = _currentLineNumber;
        _profilePointCount = 0;

        bool ok;
        int textureIndex = readNextLine().toInt(&ok);
        if(!ok)
            throwIssue(OTContentValidatorIssue::InvalidIntegerValue, {_currentLine});
        else {
            _textureAssignments[_lastProfileStart] = textureIndex;
        }

        return;
    }

    // profile point definition
    if(_currentLine == "[profilepnt]") {
        _splineTextureFound = false;
        _profilePointCount++;
        if(_lastProfileStart == 0)
            throwIssue(OTContentValidatorIssue::ProfilePointWithoutProfile);

        for(int i = 0; i < 4; i++) {
            if(!isValidFloat(readNextLine()))
                throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});
        }

        return;
    }

    // path definition
    if(_currentLine == "[path]" || _currentLine == "[path_2]") {
        _splineTextureFound = false;
        bool path2 = _currentLine == "[path_2]";
        bool ok;
        int pathType = readNextLine().toInt(&ok);
        if(!ok || pathType < 0 || pathType > 3)
            throwIssue(OTContentValidatorIssue::InvalidPathType, {_currentLine});

        for(int i = 0; i < 3; i++)
            if(!isValidFloat(readNextLine()))
                throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});

        int pathDirection = readNextLine().toInt(&ok);
        if(!ok || pathDirection < 0 || pathDirection > 2)
            throwIssue(OTContentValidatorIssue::InvalidPathDirection, {_currentLine});

        if(path2)
            if(!isValidFloat(readNextLine()))
                throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});

        return;

    }
}

void OTSplineValidator::finalizeValidation() {
    checkLastProfilePointCount();

    for (auto it = _textureAssignments.begin(); it != _textureAssignments.end(); it++)
        if(it.value() < 0 || it.value() >= _textureCount)
            throwIssueAtLine(it.key() + 1, OTContentValidatorIssue::ProfileTextureIndexOutOfRange, {QString::number(_textureCount - 1)});
}

void OTSplineValidator::checkLastProfilePointCount() {
    if(_lastProfileStart != 0 && _profilePointCount < 2)
        throwIssueAtLine(_lastProfileStart, OTContentValidatorIssue::TooFewProfilePoints, {QString::number(_profilePointCount)});
}
