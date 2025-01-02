#include "OTVehicleValidator.h"

#include <QTextStream>

OTVehicleValidator::OTVehicleValidator(QObject *parent, const QString &filePath) :
    OTModelConfigValidator(parent, filePath) {}

void OTVehicleValidator::specificValidate() {
    while(!_stream->atEnd()) {
        readNextLine();
        validateLine();
        OTModelConfigValidator::validateLine();
    }
}

void OTVehicleValidator::validateLine() {
    // TODO: Implement vehicle validation
}
