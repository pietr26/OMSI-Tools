#ifndef OTVEHICLEVALIDATOR_H
#define OTVEHICLEVALIDATOR_H

#include "OTModelConfigValidator.h"

class OTVehicleValidator : public OTModelConfigValidator
{
    Q_OBJECT
public:
    explicit OTVehicleValidator(QObject *parent, const QString &filePath);

protected:
    void specificValidate() override;
    void validateLine() override;
};

#endif // OTVEHICLEVALIDATOR_H
