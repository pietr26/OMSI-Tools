#include "OTModelConfigValidator.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>

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
}

void OTModelConfigValidator::finalizeValidation() {
    for (QHash<int, QString>::const_iterator it = _foundVariables.constBegin(); it != _foundVariables.constEnd(); ++it)
        if(!_definedVariables.contains(it.value()))
            throwIssueAtLine(it.key(), OTContentValidatorIssue::MissingVariable, {it.value()});

    for (QHash<int, QString>::const_iterator it = _foundStringVariables.constBegin(); it != _foundStringVariables.constEnd(); ++it)
        if(!_definedStringVariables.contains(it.value()))
            throwIssueAtLine(it.key(), OTContentValidatorIssue::MissingStringVariable, {it.value()});
}

void OTModelConfigValidator::readVarlist(const QString &filePath, const bool &stringvarlist) {
    QFile f(filePath);
    if(!f.open(QFile::ReadOnly | QFile::Text)) {
        qWarning().noquote() << "Couldn't open file" << filePath << "for reading:" << f.errorString();
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
