#include "OTSplineValidator.h"

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
}
