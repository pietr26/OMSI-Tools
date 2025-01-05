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
    void finalizeValidation() override;

    void checkLastProfilePointCount();

    int _textureCount = 0;
    QHash<int, int> _textureAssignments;
    int _lastProfileStart = 0;
    int _profilePointCount = 0;
};

#endif // OTSPLINEVALIDATOR_H
