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
    QTextStream s(&f);
    if(_filePath.endsWith(".map"))
        _stream->setEncoding(QStringConverter::Utf16LE);
    else
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
