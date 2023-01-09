#include "OTLogger.h"

QString Logger::filename;
bool Logger::logging = false;
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);
unsigned int entryCount = 1;
int logfileMode;
bool hardcoreDebugLogfile;

Logger::Logger(QObject *parent) : QObject(parent)
{

}

/// Creates the start of a logfile
void Logger::attach(QString filename)
{
    OTSettings set;

    Logger::filename = QDir::currentPath() + QDir::separator() + filename;
    logfileMode = set.read("main", "logfileMode").toInt();

    Logger::logging = true;
    qInstallMessageHandler(Logger::handler);

    QFile file(Logger::filename);

    QString debugInfo;
    switch (logfileMode)
    {
        case -1: debugInfo = "Logging disabled. You should not see this text. ;-)"; break;
        case 0: debugInfo = "Normal logging"; break;
        case 1: debugInfo = "Debug logging"; break;
        case 2: debugInfo = "Detail debug logging"; break;
    }

    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);

        out << QString("========================\n   %1 Logfile\n========================\n" + OTInformation::versions::currentVersion.first + " | Qt " + qVersion() + " | %2\n\n").arg(OTInformation::name, debugInfo);
        file.close();
    }
}

/// Prepares and writes a logfile entry
void Logger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (Logger::logging)
    {
        QString logText;
        switch (type)
        {
            case QtDebugMsg:
                if (logfileMode <= 0)
                    return;
                logText = QString("      [Debug]   %1").arg(msg);
                break;

            case QtInfoMsg:
                logText = QString("    [Info]      %1").arg(msg);
                break;

            case QtWarningMsg:
                logText = QString("  [Warning]     %1").arg(msg);
                break;

            case QtCriticalMsg:
                logText = QString("[Critical]      %1").arg(msg);
                break;

            case QtFatalMsg:
                logText = QString("[Fatal]         %1").arg("Application closed due to a fatal error: " + msg);
                break;
        }

        QFile file(Logger::filename);

        if (file.open(QFile::Append | QFile::Text))
        {
            QTextStream out(&file);

            // hardcore debug:
            if (logfileMode == 2)
            {
                out << entryCount << "\t[" << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss") << "] " << logText << "\n";

                out << "src " << context.file << "(" << context.line << ") | " << context.function << "\n\n";

            }
            else
                out << entryCount << "\t[" << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss") << "] " << logText << "\n";
            file.close();
        }
    }

    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);

    entryCount++;
}
