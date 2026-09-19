#include "OTLogger.h"

#include <QMutex>
#include <QMutexLocker>

#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include "OTInformation.h"
#include "OTPlatform.h"
#include "OTSettings.h"

QString OTLogger::filename;
bool OTLogger::logging = false;
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

/// handler() is called from every thread which uses a Qt logging macro - besides the
/// GUI thread those are OTMapScanner and OTMapChecker. Without a lock all three write
/// into the same file and increment the same counter at once.
static QMutex logMutex;

unsigned int entryCount = 1;
int logfileMode;
bool hardcoreDebugLogfile;

OTLogger::OTLogger(QObject *parent) : QObject(parent) { }

/// Creates the start of a logfile
void OTLogger::attach(QString filename, QString applicationName)
{
    OTSettings set;

    OTLogger::filename = OTPlatform::applicationFile(filename);
    logfileMode = set.read("main", "logfileMode", false).toInt();

    OTLogger::logging = true;
    qInstallMessageHandler(OTLogger::handler);

    QFile file(OTLogger::filename);

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
        QString hashs = "################";

        for (int i = 0; i < applicationName.length(); i++)
            hashs += "#";

        out << QString(hashs + "\n   " + applicationName + " - Logfile\n" + hashs + "\n" + OTInformation::versions::currentVersion.first + " | Qt " + qVersion() + " | %1\n\n").arg(debugInfo);
        file.close();
    }
}

/// Prepares and writes a logfile entry
void OTLogger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logText;

    if (OTLogger::logging)
    {
        switch (type)
        {
            case QtDebugMsg:
                if (logfileMode <= 0)
                    return;
                logText = QString("      [Debug]   %1").arg(msg);
                break;

            case QtInfoMsg:
                logText = QString("     [Info]     %1").arg(msg);
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
    }

    QMutexLocker locker(&logMutex);

    if (OTLogger::logging)
    {
        QFile file(OTLogger::filename);

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
