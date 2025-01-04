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
    QString shortDesc, longDesc, bothDesc;
    switch(e) {
    case FileMissing:
        shortDesc = QObject::tr("File not found");
        longDesc  = QObject::tr("File %1 not found").arg(args.isEmpty() ? "" : args[0]);
        break;
    case FileNotOpen:
        shortDesc = QObject::tr("Could not open file");
        longDesc  = QObject::tr("Could not open file %1: %2").arg(args.isEmpty() ? "" : args[0]).arg(args.count() < 2 ? "unknown error" : args[1]);
        break;
    case InvalidIntegerValue:
        shortDesc = QObject::tr("Invalid integer value");
        longDesc  = QObject::tr("Invalid integer value: %1").arg(args.isEmpty() ? "" : args[0]);
        break;
    case InvalidFloatValue:
        shortDesc = QObject::tr("Invalid float value");
        longDesc  = QObject::tr("Invalid float value: %1").arg(args.isEmpty() ? "" : args[0]);
        break;
    case MissingMeshFile:
        shortDesc = QObject::tr("Missing mesh file");
        longDesc  = QObject::tr("Missing mesh file: %1").arg(args.isEmpty() ? "" : args[0]);
        break;
    case MissingScriptFile:
        shortDesc = QObject::tr("Missing script file");
        longDesc  = QObject::tr("Missing script file: %1").arg(args.isEmpty() ? "" : args[0]);
        break;
    case MissingTextureFile:
        shortDesc = QObject::tr("Missing texture file");
        longDesc  = QObject::tr("Missing texture file: %1").arg(args.isEmpty() ? "" : args[0]);
        break;
    case MeshConfigWithoutMesh:
        bothDesc =  QObject::tr("Mesh configuration without leading [mesh]");
        break;
    case MaterialWithoutMesh:
        bothDesc =  QObject::tr("Material definition without leading [mesh]");
        break;
    case MaterialPropertyWithoutMaterial:
        bothDesc =  QObject::tr("Material property definition without leading [matl]/[matl_change]");
        break;
    case ProfileTextureIndexOutOfRange:
        shortDesc = QObject::tr("Invalid texture index");
        longDesc  = QObject::tr("Texture index is out of range: Min: 0; Max: %1").arg(args.isEmpty() ? "unkown" : args[0]);
        break;
    case ProfilePointWithoutProfile:
        bothDesc =  QObject::tr("Profile point without leading [profile]");
        break;
    case TooFewProfilePoints:
        shortDesc = QObject::tr("Too few profile points");
        longDesc  = QObject::tr("This profile doesn't have enough profile points (has %1, needs at least 2)").arg(args.isEmpty() ? "unkown" : args[0]);
        break;
    case InvalidPathType:
        shortDesc = QObject::tr("Invalid path type");
        longDesc  = QObject::tr("\"%1\" is not a valid path type (valid: 0; 1; 2; 3)").arg(args.isEmpty() ? "unkown" : args[0]);
        break;
    case InvalidPathDirection:
        shortDesc = QObject::tr("Invalid path direction");
        longDesc  = QObject::tr("\"%1\" is not a valid path direction (valid: 0; 1; 2)").arg(args.isEmpty() ? "unkown" : args[0]);
        break;
    default:
        qDebug() << "Missing issue description for " << e;
        bothDesc = QObject::tr("Unknown issue");
    }

    if(bothDesc.isEmpty())
        return QPair<QString, QString>(shortDesc, longDesc);
    else
        return QPair<QString, QString>(bothDesc, bothDesc);
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

QHash<int, QString> OTContentValidatorResult::linkedFiles() const {
    return _linkedFiles;
}

void OTContentValidatorResult::addLinkedFile(const int &lineNumber, const QString &filePath) {
    _linkedFiles[lineNumber] = filePath;
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

void OTContentValidator::throwIssueAtLine(const int &lineNumber,
                                    const OTContentValidatorIssue::IssueType &errorType,
                                    const QStringList &arguments) {
    _result << OTContentValidatorIssue(lineNumber, errorType, arguments);
}

void OTContentValidator::throwIssue(const OTContentValidatorIssue::IssueType &errorType, const QStringList &arguments) {
    throwIssueAtLine(_currentLineNumber, errorType, arguments);
}

void OTContentValidator::addLinkedFile(const int &lineNumber, const QString &filePath) {
    _result.addLinkedFile(lineNumber, filePath);
}

void OTContentValidator::addLinkedFile(const QString &filePath) {
    addLinkedFile(_currentLineNumber, filePath);
}

bool OTContentValidator::isValidInt(const QString &string) {
    bool ok;
    string.toInt(&ok);
    return ok;
}

bool OTContentValidator::isValidFloat(const QString &string) {
    bool ok;
    string.toFloat(&ok);
    return ok;
}
