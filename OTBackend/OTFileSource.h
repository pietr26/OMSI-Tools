#ifndef OTFILESOURCE_H
#define OTFILESOURCE_H

#include <QObject>

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

    OTFileSource(const QString &fileName, FileType fileType);

    QString fileName() const;
    FileType fileType() const;

    QStringList sources();
    int sourcesCount();
    void addSource(const QString &newSource);
    void addSources(const QStringList &newSources);

    int occurrencesCount() const;
    void newOccurrence();

    QString errorString() const;

    bool hasMissingMesh() const;
    QStringList missingMeshs() const;
    void addMissingMesh(const QString &newMesh);

    bool hasMissingScriptFile() const;
    QStringList missingScriptFiles() const;
    void addMissingScriptFile(const QString &newScriptFile);

    bool hasMissingTexture();
    QStringList missingTextures() const;
    void addMissingTexture(const QString &newTexture);


private:
    QString _fileName;
    QStringList _sources;
    QStringList _missingMeshs, _missingScriptFiles, _missingTextures;
    FileType _fileType;
    int _occurrences;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(OTFileSource::ErrorCodes);

#endif // OTFILESOURCE_H
