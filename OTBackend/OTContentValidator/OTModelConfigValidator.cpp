#include "OTModelConfigValidator.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>

#include "OTBackend/OTOmsiFileHandler.h"

OTModelConfigValidator::OTModelConfigValidator(QObject *parent, const QString &filePath) :
    OTContentValidator(parent, filePath) {
    //TODO: init predefined variables here for bus and sco
}

void OTModelConfigValidator::specificValidate() {
    while(!_stream->atEnd()) {
        readNextLine();
        validateLine();
    }
}

void OTModelConfigValidator::validateLine() {
    // missing mesh files
    if(_currentLine == "[mesh]" || _currentLine == "[collision_mesh]") {
        _meshFound = true;
        _matlFound = false;
        QString mesh = readNextLine();
        if(!QFile::exists(_fileDir + "/model/" + mesh.trimmed())) {
            throwIssue(OTContentValidatorIssue::MissingMeshFile, {mesh});
            return;
        } else {
            addLinkedFile(_fileDir + "/model/" + mesh);
        }

        return;
    }

    // materials
    if(_currentLine == "[matl]" || _currentLine == "[matl_change]") {
        checkLastMaterial();
        bool isMatlChange = _currentLine == "[matl_change]";
        _matlFound = true;
        _matlItemFound = false;
        _matlIsTexttexture = false;
        _lastMatl = QPair<int, int>(_currentLineNumber, isMatlChange ? 2 : 1);
        if(!_meshFound)
            throwIssue(OTContentValidatorIssue::MaterialWithoutMesh, {});

        _lastMatlTexture = readNextLine(); // texture

        if(!isValidInt(readNextLine())) // submesh index
            throwIssue(OTContentValidatorIssue::InvalidIntegerValue, {_currentLine});

        if(isMatlChange) // changevar
            _foundVariables.insert(_currentLineNumber, readNextLine());

        return;
    }
    if(_currentLine == "[matl_item]") {
        _matlItemFound = true;
        if(_matlFound && _lastMatl.second == 1)
            throwIssue(OTContentValidatorIssue::MaterialItemWithoutMaterialChange);
    }
    if(_matlProperties.contains(_currentLine) && !_matlFound) {
        throwIssue(OTContentValidatorIssue::MaterialPropertyWithoutMaterial, {});
        return;
    }

    // missing script files
    if(_currentLine == "[varnamelist]" || _currentLine == "[stringvarnamelist]" || _currentLine == "[script]" || _currentLine == "[constfile]") {
        int includeVarlist = _currentLine == "[varnamelist]" ? 1 : _currentLine == "[stringvarnamelist]" ? 2 : 0;
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
            if(!QFile::exists(_fileDir + "/" + _currentLine.trimmed())) {
                throwIssue(OTContentValidatorIssue::MissingScriptFile, {_currentLine});
            } else {
                addLinkedFile(_fileDir + "/" + _currentLine);
                if(includeVarlist)
                    readVarlist(_fileDir + "/" + _currentLine, includeVarlist == 2);
            }
            i++;
            readNextLine();
        }

        return;
    }

    // texttexture
    if(_currentLine == "[texttexture]" || _currentLine == "[texttexture_enh]") {
        _texttextureCount++;
        bool enhanced = _currentLine == "[texttexture_enh]";

        // string variable
        if(!isValidInt(readNextLine())) // skip if a string index is given instead of a variable (in sceneryobjecs)
            _foundStringVariables.insert(_currentLineNumber, _currentLine);

        // TODO: Check if font exists
        readNextLine();
        if(!isValidInt(readNextLine())) // width
            throwIssue(OTContentValidatorIssue::InvalidIntegerValue, {_currentLine});
        if(!isValidInt(readNextLine())) // height
            throwIssue(OTContentValidatorIssue::InvalidIntegerValue, {_currentLine});

        bool ok;
        int colorCode = readNextLine().toInt(&ok);
        if(!ok) // color mode
            throwIssue(OTContentValidatorIssue::InvalidIntegerValue, {_currentLine});
        else if(colorCode < 0 || colorCode > 1)
            throwIssue(OTContentValidatorIssue::InvalidTexttextureColorMode, {_currentLine});

        for(int i = 0; i < 3; i++) // color
            if(!isValidRgbValue(readNextLine()))
                throwIssue(OTContentValidatorIssue::InvalidRgbValue, {_currentLine});

        if(enhanced) {
            bool ok;
            int val = readNextLine().toInt(&ok);
            if(!ok || val < 0 || val > 5)
                throwIssue(OTContentValidatorIssue::InvalidTexttextureAlignment);

            val = readNextLine().toInt(&ok);
            if(!ok || val < 0 || val > 1)
                throwIssue(OTContentValidatorIssue::InvalidTexttexturePixelAlignment);
        }
        return;
    }

    // scripttexture
    if(_currentLine == "[scripttexture") {
        for(int i = 0; i < 2; i++)
            if(!isValidFloat(readNextLine()))
                throwIssue(OTContentValidatorIssue::InvalidIntegerValue, {_currentLine});
        return;
    }

    // texttexture assignment
    if(_currentLine == "[useTextTexture]") {
        _matlIsTexttexture = true;
        bool ok;
        int val = readNextLine().toInt(&ok);
        if(!ok)
            throwIssue(OTContentValidatorIssue::InvalidIntegerValue, {_currentLine});
        else
            _texttextureAssignments.insert(_currentLineNumber, val);
    }
}

void OTModelConfigValidator::finalizeValidation() {
    checkLastMaterial();

    // check variables
    for (QHash<int, QString>::const_iterator it = _foundVariables.constBegin(); it != _foundVariables.constEnd(); ++it)
        if(!_definedVariables.contains(it.value()))
            throwIssueAtLine(it.key(), OTContentValidatorIssue::MissingVariable, {it.value()});

    for (QHash<int, QString>::const_iterator it = _foundStringVariables.constBegin(); it != _foundStringVariables.constEnd(); ++it)
        if(!_definedStringVariables.contains(it.value()))
            throwIssueAtLine(it.key(), OTContentValidatorIssue::MissingStringVariable, {it.value()});

    // check texttexture assignments
    for (auto it = _texttextureAssignments.begin(); it != _texttextureAssignments.end(); it++)
        if(it.value() < 0 || it.value() >= _texttextureCount)
            throwIssueAtLine(it.key(), OTContentValidatorIssue::TexttextureIndexOutOfRange, {QString::number(_texttextureCount - 1)});
}

void OTModelConfigValidator::readVarlist(const QString &filePath, const bool &stringvarlist) {
    QFile f(filePath);
    if(!f.open(QFile::ReadOnly | QFile::Text)) {
        throwIssue(OTContentValidatorIssue::FileNotOpen, {filePath, f.errorString()});
        return;
    }

    QStringList *list = stringvarlist ? &_definedStringVariables : &_definedVariables;

    QTextStream s(&f);
    while(!s.atEnd()) {
        QString line = s.readLine();
        if(line.trimmed().isEmpty())
            continue;

        list->append(line);
    }

    list->removeDuplicates();

    f.close();
}

void OTModelConfigValidator::checkLastMaterial() {
    if(!_matlFound)
        return;

    if(_lastMatl.second == 2 && !_matlItemFound)
        throwIssueAtLine(_lastMatl.first, OTContentValidatorIssue::MaterialChangeWithoutItem);

    // check texture assignment
    if(!_matlIsTexttexture) {
        if(!OTOMSIFileHandler::checkTexture(_fileDir + "/texture/" + _lastMatlTexture.trimmed(), _lastMatlTexture.trimmed()))
            throwIssueAtLine(_lastMatl.first + 1, OTContentValidatorIssue::MissingTextureFile, {_lastMatlTexture});
    }
}
