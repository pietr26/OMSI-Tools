#include "OTSceneryobjectValidator.h"

#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include "OTBackend/OTOmsiFileHandler.h"

OTSceneryobjectValidator::OTSceneryobjectValidator(QObject *parent, const QString &filePath) :
    OTModelConfigValidator(parent, filePath) {}

void OTSceneryobjectValidator::specificValidate() {
    while(!_stream->atEnd()) {
        readNextLine();
        validateLine();
        OTModelConfigValidator::validateLine();
    }
}

void OTSceneryobjectValidator::validateLine() {
    /* TODO:
     * external model.cfg support
     * passengercabin.cfg
     */

    // validate [tree]
    if(_currentLine == "[tree]") {
        readNextLine();
        if(!OTOMSIFileHandler::checkTexture(_fileDir + "/texture/" + _currentLine, _currentLine))
            throwIssue(OTContentValidatorIssue::MissingTextureFile, {_currentLine});

        // check numeric values
        bool ok;
        readNextLine().toFloat(&ok);
        if(!ok)
            throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});
        readNextLine().toFloat(&ok);
        if(!ok)
            throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});
        readNextLine().toFloat(&ok);
        if(!ok)
            throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});
        readNextLine().toFloat(&ok);
        if(!ok)
            throwIssue(OTContentValidatorIssue::InvalidFloatValue, {_currentLine});

        return;
    }
}
