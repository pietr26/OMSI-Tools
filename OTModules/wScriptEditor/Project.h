#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QSet>
#include <QRegularExpression>

class Project : public QObject {
    Q_OBJECT
public:
    explicit Project(QObject *parent = nullptr);

    enum Type {
        SceneryobjectType,
        HumanType,
        VehicleType
    };

    enum ScriptFileType {
        VarlistType,
        StringVarlistType,
        ScriptType,
        ConstfileType
    };

    QString path() const;
    void setPath(const QString &newPath);

    QSet<QString> varlistFiles() const;
    QSet<QString> stringVarlistFiles() const;
    QSet<QString> scriptFiles() const;
    QSet<QString> constFiles() const;

protected:
    void readFile();

    void readProjectFiles();

    QStringList readVarlist(const QString &filePath);
    QMap<QString, float> readConstfile(const QString &filePath);

signals:
    void reset();

    void varlistFileAdded(const QString &filePath);
    void varlistFileRemoved(const QString &filePath);

    void stringVarlistFileAdded(const QString &filePath);
    void stringVarlistFileRemoved(const QString &filePath);

    void scriptFileAdded(const QString &filePath);
    void scriptFileRemoved(const QString &filePath);

    void constFileAdded(const QString &filePath);
    void constFileRemoved(const QString &filePath);

    void varAdded(const QString &varname);
    void varRemoved(const QString &varname);

    void stringVarAdded(const QString &varname);
    void stringVarRemoved(const QString &varname);

    void constAdded(const QString &constname, const float &value);
    void constRemoved(const QString &constname, const float &value);

private:
    QString _path, _dir;
    QSet<QString> _varlistFiles, _stringVarlistFiles, _scriptFiles, _constFiles;
    QSet<QString> _vars, _stringVars;
    QMap<QString, float> _consts;

    static inline QList<QString> _commands = {"[varnamelist]", "[stringvarnamelist]", "[script]", "[constfile]"};
};

#endif // PROJECT_H
