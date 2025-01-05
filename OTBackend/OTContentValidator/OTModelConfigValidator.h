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
    void readMaterial(const bool &isSplineTexture = false);
    void checkLastMaterial();

    bool _meshFound = false, _matlFound = false, _matlItemFound = false, _matlIsTexttexture = false;
    QPair<int, int> _lastMatl = QPair<int, int>(0, 0); //<line number, type> (0 = invalid/unspecified, 1 = matl/texture (spline), 2 = matl_change)
    QString _lastMatlTexture;

    bool _splineScanning = false, _splineTextureFound = false;

    QHash<int, QString> _foundVariables, _foundStringVariables;
    QStringList _definedVariables, _definedStringVariables;

    int _texttextureCount = 0;
    QHash<int, int> _texttextureAssignments;

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
        "[matl_noZcheck]",
        "[matl_noZwrite]",
        "[matl_texadress_border]",
        "[matl_texadress_clamp]",
        "[matl_transmap]"
    };
};

#endif // OTMODELCONFIGVALIDATOR_H
