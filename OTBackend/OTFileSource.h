#ifndef OTFILESOURCE_H
#define OTFILESOURCE_H

#include <QObject>
#include "OTBackend/OTContentValidator/OTContentValidator.h"

class OTFileSource {
public:
    enum ErrorCode {
        MeshMissing = 0x01,
        VarlistMissing = 0x02,
        TextureMissing = 0x04
    };
    Q_DECLARE_FLAGS(ErrorCodes, ErrorCode);

    enum FileType {
        SceneryobjectFile,
        SplineFile,
        HumanFile,
        VehicleFile,
        TileFile,
        TextureFile,
        UnknownFile
    };

    OTFileSource(const QString &fileName = "", FileType fileType = UnknownFile);
    bool operator==(const OTFileSource &other);
    bool operator<(const OTFileSource &other);

    QString fileName() const;
    FileType fileType() const;

    QList<QPair<QString, int>> sources();
    QStringList sourceStrings();
    int sourcesCount();
    int occurrencesCount() const;
    void addOccurrence(const QString &newSource);

    OTContentValidatorResult advancedCheckResult() const;
    void setAdvancedCheckResult(const OTContentValidatorResult &result);

private:
    QString _fileName;
    QList<QPair<QString, int>> _sources;
    FileType _fileType;
    OTContentValidatorResult _advancedCheckResult;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(OTFileSource::ErrorCodes);

#endif // OTFILESOURCE_H
