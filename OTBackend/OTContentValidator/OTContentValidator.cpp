#include "OTContentValidator.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "OTBackend/OTGlobal.h"

OTContentValidatorIssue::OTContentValidatorIssue(const int &lineNumber,
                                                 const int &issueType,
                                                 const QStringList &arguments) :
    _lineNumber(lineNumber),
    _issueType(issueType),
    _arguments(arguments),
    _issueDescription(issueDescription(issueType, arguments)) {
}

int OTContentValidatorIssue::lineNumber() const {
    return _lineNumber;
}

QString OTContentValidatorIssue::shortissueDescription() const {
    return _issueDescription.first;
}

QString OTContentValidatorIssue::longissueDescription() const {
    return _issueDescription.second;
}

QPair<QString, QString> OTContentValidatorIssue::issueDescription(const int &e, const QStringList &args) const {
    switch(e) {
    case FileMissing:
        return QPair<QString, QString>(
            QObject::tr("File not found"),
            QObject::tr("File %1 not found").arg(args.isEmpty() ? "" : args[0])
        );
    case FileNotOpen:
        return QPair<QString, QString>(
            QObject::tr("Could not open file"),
            QObject::tr("Could not open file %1: %2").arg(args.isEmpty() ? "" : args[0]).arg(args.count() < 2 ? "unkown error" : args[1])
        );
    case InvalidIntegerValue:
        return QPair<QString, QString>(
            QObject::tr("Invalid integer value"),
            QObject::tr("Invalid integer value: %1").arg(args.isEmpty() ? "" : args[0])
        );
    case InvalidFloatValue:
        return QPair<QString, QString>(
            QObject::tr("Invalid float value"),
            QObject::tr("Invalid float value: %1").arg(args.isEmpty() ? "" : args[0])
        );
    case MissingMeshFile:
        return QPair<QString, QString>(
            QObject::tr("Missing mesh file"),
            QObject::tr("Missing mesh file: %1").arg(args.isEmpty() ? "" : args[0])
        );
    case MissingScriptFile:
        return QPair<QString, QString>(
            QObject::tr("Missing script file"),
            QObject::tr("Missing script file: %1").arg(args.isEmpty() ? "" : args[0])
        );
    case MissingTextureFile:
        return QPair<QString, QString>(
            QObject::tr("Missing texture file"),
            QObject::tr("Missing texture file: %1").arg(args.isEmpty() ? "" : args[0])
        );
    case MeshConfigWithoutMesh:
        return QPair<QString, QString>(
            QObject::tr("Mesh configuration without leading [mesh]"),
            QObject::tr("Mesh configuration without leading [mesh]")
        );
    case MaterialWithoutMesh:
        return QPair<QString, QString>(
            QObject::tr("Material definition without leading [mesh]"),
            QObject::tr("Material definition without leading [mesh]")
        );
    case MaterialPropertyWithoutMaterial:
        return QPair<QString, QString>(
            QObject::tr("Material property definition without leading [matl]/[matl_change]"),
            QObject::tr("Material property definition without leading [matl]/[matl_change]")
        );
    default:
        qDebug() << "missing issue description for " << e;
        return QPair<QString, QString>(QObject::tr("Unkown issue"), QObject::tr("Unknown issue"));
    }
}


OTContentValidatorResult::OTContentValidatorResult(const QString &fileName) :
    _fileName(fileName) {
}

void OTContentValidatorResult::operator<<(const OTContentValidatorIssue &issue) {
    addIssue(issue);
}

QString OTContentValidatorResult::fileName() const {
    return _fileName;
}

QList<OTContentValidatorIssue> OTContentValidatorResult::issues() const {
    return _issues;
}

int OTContentValidatorResult::issueCount() const {
    return _issues.count();
}

bool OTContentValidatorResult::isValid() const {
    return _issues.isEmpty();
}

void OTContentValidatorResult::addIssue(const OTContentValidatorIssue &issue) {
    _issues << issue;
}


OTContentValidator::OTContentValidator(QObject *parent, const QString &filePath) :
    QObject(parent),
    _result(filePath),
    _filePath(filePath),
    _currentLineNumber(0),
    _stream(nullptr) {

    QFileInfo info(_filePath);
    _fileDir = info.dir().path();

    OTSettings set;
    _omsiDir = set.read("main", "mainDir").toString();
}

QString OTContentValidator::filePath() const {
    return _filePath;
}

void OTContentValidator::setFilePath(const QString &newFilePath) {
    _filePath = newFilePath;
}

void OTContentValidator::validate() {
    QFile f(_filePath);
    if(!f.exists()) {
        throwIssue(OTContentValidatorIssue::FileMissing, {_filePath});
        return;
    }

    if(!f.open(QFile::ReadOnly)) {
        throwIssue(OTContentValidatorIssue::FileNotOpen, {_filePath, f.errorString()});
        return;
    }

    _stream = new QTextStream(&f);
    _stream->setEncoding(QStringConverter::Latin1);
    specificValidate();
    f.close();
}

OTContentValidatorResult OTContentValidator::result() const {
    return _result;
}

QString OTContentValidator::readNextLine() {
    _currentLineNumber++;
    _currentLine = _stream->readLine();
    return _currentLine;
}

void OTContentValidator::throwIssue(const OTContentValidatorIssue::IssueType &errorType, const QStringList &arguments) {
    _result << OTContentValidatorIssue(_currentLineNumber, errorType, arguments);
}
