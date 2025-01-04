#ifndef OTMODELCONFIGVALIDATOR_H
#define OTMODELCONFIGVALIDATOR_H

#include "OTContentValidator.h"

class OTModelConfigValidator : public OTContentValidator
{
    Q_OBJECT
public:
    explicit OTModelConfigValidator(QObject *parent, const QString &filePath);

protected:
    void specificValidate() override;
    void validateLine() override;
    void finalizeValidation() override;

    void readVarlist(const QString &filePath, const bool &stringvarlist);

    bool _meshFound = false, _matlFound = false;

    QHash<int, QString> _foundVariables, _foundStringVariables;
    QStringList _definedVariables, _definedStringVariables;

    const QStringList _matlProperties = {
        "[matl_allcolor]",
        "[matl_alpha]",
        "[matl_bumpmap]",
        "[matl_envmap_mask]",
        "[matl_envmap]",
        "[matl_freetex]",
        "[matl_item]",
        "[matl_lightmap]",
        "[matl_nightmap]",
        "[matl_noWrite]",
        "[matl_noXwrite]",
        "[matl_noYwrite]",
        "[matl_noZcheck]",
        "[matl_noZwrite]",
        "[matl_nozWrite]",
        "[matl_nozwrite]",
        "[matl_texadress_border]",
        "[matl_texadress_clamp]",
        "[matl_transmap]"
    };
};

#endif // OTMODELCONFIGVALIDATOR_H
