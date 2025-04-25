#include "Project.h"

Project::Project(QObject *parent)
    : QObject{parent}
{}

QString Project::path() const {
    return _path;
}


QString Project::dir() const {
    return _dir;
}

void Project::setPath(const QString &newPath) {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    _path = newPath;

    QFileInfo fi(_path);
    _dir = fi.dir().path();

    _varlistFiles.clear();
    _stringVarlistFiles.clear();
    _scriptFiles.clear();
    _constFiles.clear();

    readFile();
    readProjectFiles();

    emit reset();
    QApplication::restoreOverrideCursor();
}

QSet<QString> Project::varlistFiles() const {
    return _varlistFiles;
}

QSet<QString> Project::stringVarlistFiles() const {
    return _stringVarlistFiles;
}

QSet<QString> Project::scriptFiles() const {
    return _scriptFiles;
}

QSet<QString> Project::constFiles() const {
    return _constFiles;
}

void Project::readFile() {
    QFile f(_path);

    if(!f.open(QFile::ReadOnly)) {
        qWarning().noquote() << "Cannot open file:" << _path << f.errorString();
        return;
    }
    QTextStream s(&f);
    s.setEncoding(QStringConverter::Latin1);

    while(!s.atEnd()) {
        QString line = s.readLine();
        if(_commands.contains(line)) {
            ScriptFileType type = ScriptFileType(_commands.indexOf(line));

            QString numStr = s.readLine();
            int count = numStr.toInt();
            for(int i = 0; i < count && !s.atEnd(); i++) {
                QString filePath = s.readLine().replace("\\", "/");
                switch(type) {
                    case VarlistType:
                        _varlistFiles.insert(filePath);
                        break;
                    case StringVarlistType:
                        _stringVarlistFiles.insert(filePath);
                        break;
                    case ScriptType:
                        _scriptFiles.insert(filePath);
                        break;
                    case ConstfileType:
                        _constFiles.insert(filePath);
                        break;
                }
            }
        }
    }

    f.close();
}

void Project::readProjectFiles() {
    for(const QString &filePath : std::as_const(_varlistFiles)) {
        const QStringList list = readVarlist(filePath);
        for(const QString &string : list) {
            _vars.insert(string);
            emit varAdded(string);
        }
    }

    for(const QString &filePath : std::as_const(_stringVarlistFiles)) {
        const QStringList list = readVarlist(filePath);
        for(const QString &string : list) {
            _stringVars.insert(string);
            emit stringVarAdded(string);
        }

    }

    for(const QString &filePath : std::as_const(_constFiles)) {
        const QMap<QString, float> map = readConstfile(filePath);
        const QStringList keys = map.keys();
        for(const QString &key : keys) {
            const float value = map.value(key);
            _consts.insert(key, value);
            emit constAdded(key, value);
        }
    }
}

QStringList Project::readVarlist(const QString &filePath) {
    QStringList list;
    QFile f(_dir + "/" + filePath);
    if(!f.open(QFile::ReadOnly)) {
        qWarning().noquote() << "Cannot open file:" << filePath << f.errorString();
        return {};
    }
    QTextStream s(&f);
    s.setEncoding(QStringConverter::Latin1);

    while(!s.atEnd()) {
        QString name =  s.readLine();
        name.remove(QRegularExpression("\\s"));
        if(!name.isEmpty())
            list << name;
    }

    f.close();
    return list;
}

QMap<QString, float> Project::readConstfile(const QString &filePath) {
    QMap<QString, float> map;

    QFile f(_dir + "/" + filePath);
    if(!f.open(QFile::ReadOnly)) {
        qWarning().noquote() << "Cannot open file:" << filePath << f.errorString();
        return {};
    }
    QTextStream s(&f);
    s.setEncoding(QStringConverter::Latin1);

    while(!s.atEnd()) {
        QString currentLine = s.readLine();
        if(currentLine == "[const]") {
            QString name = s.readLine();
            QString strVal = s.readLine();
            bool ok;
            int val = strVal.toFloat(&ok);
            if(!ok) {
                qWarning().noquote() << "Cannot convert value:" << strVal << "to float";
            }
            map.insert(name, val);
        }
    }

    f.close();

    return map;
}

