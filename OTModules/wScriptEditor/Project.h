#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QSet>

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

private:
    QString _path;
    QSet<QString> _varlistFiles, _stringVarlistFiles, _scriptFiles, _constFiles;

    static inline QList<QString> _commands = {"[varnamelist]", "[stringvarnamelist]", "[script]", "[constfile]"};
};

#endif // PROJECT_H
