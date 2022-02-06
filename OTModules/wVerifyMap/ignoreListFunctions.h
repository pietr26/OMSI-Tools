#ifndef IGNORELISTFUNCTIONS_H
#define IGNORELISTFUNCTIONS_H

#include "OTBackend/OTGlobal.h"

class ignoreListFunctions
{
public:

    /// \brief Writes the ignorelist
    void write(QStringList items, bool flushBeforeWrite = false)
    {
        if (flushBeforeWrite)
            ignoreList.open(QFile::WriteOnly | QFile::Text);
        else
            ignoreList.open(QFile::WriteOnly | QFile::Append);

        qDebug() << "Append ignorelist...";
        QTextStream out(&ignoreList);

        foreach (QString current, items)
            out << current << "\n";

        if (items.empty())
            out << "";

        ignoreList.close();
    }

    void write(QString item)
    {
        QStringList returnList;
        returnList << item;

        write(returnList);
    }

    /// \brief Returns a QStringList without ignored paths
    QStringList check(QStringList checkList, int &ignoreCount)
    {
        ignoreList.open(QFile::ReadOnly | QFile::Text);

        checkList.removeDuplicates();
        checkList.replaceInStrings("/", "\\");

        QStringList returnList;

        QStringList ignoreStringList = read(true);

        foreach (QString current, checkList)
        {
            if (ignoreStringList.contains(current, Qt::CaseInsensitive))
                ignoreCount++;
            else
                returnList << current;
        }

        qDebug().noquote() << checkList.count() - returnList.count() << "of" << checkList.count() << "ignored";

        ignoreList.close();

        return returnList;
    }

    /// \brief Gets the ignorelist
    QStringList read(bool isAlreadyOpen = false)
    {
        if (!isAlreadyOpen)
            ignoreList.open(QFile::ReadOnly | QFile::Text);

        qDebug() << "Read ignorelist...";

        QTextStream in(&ignoreList);

        QStringList returnList;
        while (!in.atEnd())
            returnList << in.readLine().replace("/", "\\");

        if (!isAlreadyOpen)
            ignoreList.close();

        return returnList;
    }

private:
    QFile ignoreList = QFile("ignorelist.cfg");
};

#endif // IGNORELISTFUNCTIONS_H
