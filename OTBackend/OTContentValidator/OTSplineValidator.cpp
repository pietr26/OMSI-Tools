#include "OTSplineValidator.h"

#include "OTBackend/OTOMSIFileHandler.h"

#include <QFile>
#include <QTextStream>

OTSplineValidator::OTSplineValidator(QObject *parent, const QString &filePath) :
    OTModelConfigValidator(parent, filePath) {}

void OTSplineValidator::specificValidate() {
    while(!_stream->atEnd()) {
        readNextLine();
        validateLine();
        OTModelConfigValidator::validateLine();
    }
}

void OTSplineValidator::validateLine() {
    // TODO: Implement spline validation
    if(_currentLine == "[texture]") {
        _meshFound = true;
        _matlFound = true;
        readNextLine();
        if(!OTOMSIFileHandler::checkTexture(_fileDir + "/texture/" + _currentLine, _currentLine))
            throwIssue(OTContentValidatorIssue::MissingTextureFile, {_currentLine});
        else
            addLinkedFile(_fileDir + "/texture/" + _currentLine);
    }
}
