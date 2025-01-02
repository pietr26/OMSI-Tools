#ifndef OTSCENERYOBJECTVALIDATOR_H
#define OTSCENERYOBJECTVALIDATOR_H

#include <QObject>

#include "OTModelConfigValidator.h"

class OTSceneryobjectValidator : public OTModelConfigValidator {
    Q_OBJECT
public:
    explicit OTSceneryobjectValidator(QObject *parent, const QString &filePath);

protected:
    void specificValidate() override;
    void validateLine() override;
};

#endif // OTSCENERYOBJECTVALIDATOR_H
