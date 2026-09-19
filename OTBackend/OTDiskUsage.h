#ifndef OTDISKUSAGE_H
#define OTDISKUSAGE_H

#include <QDir>
#include <QFileInfo>
#include <QObject>
#include <QStringList>

class OTDiskUsage
{
public:
    QString formatSize(QString dirPath)
    {
        qint64 size = calcDirSize(dirPath);

        int i = 0;
        double outputSize = size;
        for(i = 0; i < units.size() - 1; i++)
        {
            if (outputSize < 1024)
                break;

            outputSize = outputSize / 1024;
        }

        return QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);;
    }

    QString calculateSize(qint64 size)
    {
        int i = 0;
        double outputSize = size;
        for(i = 0; i < units.size() - 1; i++)
        {
            if (outputSize < 1024)
                break;

            outputSize = outputSize / 1024;
        }
        QString result = QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);
        return result;
    }

private:
    qint64 calcDirSize(QString dirPath)
    {
        qint64 size = 0;
        QDir dir(dirPath);

        // calculate total size of current directories' files
        QDir::Filters fileFilters = QDir::Files | QDir::System | QDir::Hidden;
        for (QString filePath : dir.entryList(fileFilters))
        {
            QFileInfo fi(dir, filePath);
            size += fi.size();
        }

        // add size of child directories recursively
        QDir::Filters dirFilters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden;

        for (QString childDirPath : dir.entryList(dirFilters))
            size += calcDirSize(dirPath + QDir::separator() + childDirPath);

        return size;
    }

    QStringList units = { QObject::tr("Bytes"),
                          QObject::tr("kB", "Short version of 'Kilobyte'"),
                          QObject::tr("MB", "Short version of 'Megabyte'"),
                          QObject::tr("GB", "Short version of 'Gigabyte'"),
                          QObject::tr("TB", "Short version of 'Terrabyte'"),
                          QObject::tr("PB", "Short version of 'Petabyte'")};
};

#endif // OTDISKUSAGE_H
