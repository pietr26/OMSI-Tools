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

    // Patchwork chain
    if(_currentLine == "[patchwork_chain]") {
        if(!_splineTextureFound)
            throwIssue(OTContentValidatorIssue::PatchworkChainWithoutTexture);

        if(!isValidFloat(readNextLine()))
            throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});
        int sectionCount = readNextLine().length() - 1;
        readNextLine();
        bool wrongLength = _currentLine.length() != sectionCount;
        bool invalidChars = !std::all_of(_currentLine.begin(), _currentLine.end(), [](QChar c){return c.isDigit();});

        if(wrongLength || invalidChars)
            throwPatchworkChainIssue(OTContentValidatorIssue::InvalidPatchworkChainFrequencyString,
                                     QPair<int, int>(sectionCount, _currentLine.length()),
                                     invalidChars);

        readNextLine();
        wrongLength = _currentLine.length() != sectionCount;
        invalidChars = !std::all_of(_currentLine.begin(), _currentLine.end(), [](QChar c){return c == "0" || c == "1";});

        if(wrongLength || invalidChars)
            throwPatchworkChainIssue(OTContentValidatorIssue::InvalidPatchworkChainInvertableString,
                                     QPair<int, int>(sectionCount, _currentLine.length()),
                                     invalidChars);
    }

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

void OTSplineValidator::throwPatchworkChainIssue(const OTContentValidatorIssue::IssueType &issueType,
                                                 const QPair<int, int> &lengthData,
                                                 const bool &invalidChars) {
    QStringList args;
    if(lengthData.first != lengthData.second)
        args << tr("Invalid string length (should be %1, is %2)").arg(lengthData.first).arg(lengthData.second);

    if(invalidChars)
        args << tr("Contains invalid chars (only \"%1\" allowed)").arg(issueType == OTContentValidatorIssue::InvalidPatchworkChainFrequencyString ? "0123456789" : "0 or 1");

    throwIssue(issueType, args);
}
