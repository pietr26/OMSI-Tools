#ifndef OTLOGGER_H
#define OTLOGGER_H

#include <QObject>
#include "OTGlobal.h"

class OTLogger : public QObject
{
    Q_OBJECT
public:
    explicit OTLogger(QObject *parent = nullptr);

    static bool logging;
    static QString filename;
    static void attach(QString filename, QString applicationName);
    static void handler(QtMsgType type, const QMessageLogContext &context, const QString & msg);

};

#endif // OTLOGGER_H
