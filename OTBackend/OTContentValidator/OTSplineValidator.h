#ifndef OTSPLINEVALIDATOR_H
#define OTSPLINEVALIDATOR_H

#include "OTModelConfigValidator.h"

class OTSplineValidator : public OTModelConfigValidator {
    Q_OBJECT
public:
    explicit OTSplineValidator(QObject *parent, const QString &filePath);

protected:
    void specificValidate() override;
    void validateLine() override;
};

#endif // OTSPLINEVALIDATOR_H
