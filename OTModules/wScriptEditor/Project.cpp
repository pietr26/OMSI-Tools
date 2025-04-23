#include "Project.h"

Project::Project(QObject *parent)
    : QObject{parent}
{}

QString Project::path() const {
    return _path;
}

void Project::setPath(const QString &newPath) {
    _path = newPath;

    _varlistFiles.clear();
    _stringVarlistFiles.clear();
    _scriptFiles.clear();
    _constFiles.clear();

    readFile();

    emit reset();
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
