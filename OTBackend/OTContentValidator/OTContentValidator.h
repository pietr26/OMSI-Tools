#ifndef OTCONTENTVALIDATOR_H
#define OTCONTENTVALIDATOR_H

#include <QObject>
#include <QHash>

/* The OTContentValidator class is a class that is used to validate any kind of OMSI config files (like sco, sli, cfg, bus etc.).
 * This is the base class which implements the general functionality for all validators.
 * Then for every particular file type (like sco, sli, cfg, bus etc.) there is a subclass that implements the specific validation rules.
 * The base class itself is abstract, so you need to subclass it for every file type to use it.
 * A more detailed explation is planned... :D
 *
 * The OTContentValidatorResult represents the result of a file validation. It's basically just a wrapper for a QList<OTContentValidatorIssue>
 * with some extra functionality
 *
 * The OTContentValidatorIssue class is a class that holds one issue that occured during the validation process.
 * It holds the line of the file where the issue occured, the type of the issue and maybe some additional arguments that are used to
 * generate a more individual description.
 *
 */

class OTContentValidatorIssue {
public:
    OTContentValidatorIssue(const int &lineNumber,
                            const int &issueType,
                            const QStringList &args = {});

    enum IssueType {
        // General issues
        FileMissing,
        FileNotOpen,
        InvalidIntegerValue,
        InvalidFloatValue,

        // Modelconfig issues
        MissingMeshFile,
        MissingScriptFile,
        MissingTextureFile,
        MeshConfigWithoutMesh,
        MaterialWithoutMesh,
        MaterialPropertyWithoutMaterial
    };

    int lineNumber() const;

    QString shortissueDescription() const;
    QString longissueDescription() const;

protected:
    QPair<QString, QString> issueDescription(const int &e, const QStringList &arguments = {}) const;

private:
    int _lineNumber;
    int _issueType;
    QStringList _arguments;
    QPair<QString, QString> _issueDescription;
};

class OTContentValidatorResult {
public:
    OTContentValidatorResult(const QString &fileName = "");
    void operator<<(const OTContentValidatorIssue &issue);

    QString fileName() const;

    QList<OTContentValidatorIssue> issues() const;
    int issueCount() const;
    bool isValid() const;

    void addIssue(const OTContentValidatorIssue &issue);

    QHash<int, QString> linkedFiles() const;
    void addLinkedFile(const int &lineNumber, const QString &filePath);

private:
    QString _fileName;
    QList<OTContentValidatorIssue> _issues;
    QHash<int, QString> _linkedFiles;
};

class OTContentValidator : public QObject {
    Q_OBJECT
public:
    explicit OTContentValidator(QObject *parent, const QString &filePath);

    QString filePath() const;
    void setFilePath(const QString &newFilePath);

    void validate();

    OTContentValidatorResult result() const;

protected:
    virtual void specificValidate() = 0;
    virtual void validateLine() = 0;
    QString readNextLine();
    void throwIssueAtLine(const int &lineNumber, const OTContentValidatorIssue::IssueType &errorType, const QStringList &args = {});
    void throwIssue(const OTContentValidatorIssue::IssueType &errorType, const QStringList &args = {});
    void addLinkedFile(const int &lineNumber, const QString &filePath);
    void addLinkedFile(const QString &filePath);

    OTContentValidatorResult _result;

    QString _filePath, _fileDir, _omsiDir;
    QString _currentLine;
    int _currentLineNumber;
    QTextStream *_stream;
};

#endif // OTCONTENTVALIDATOR_H
