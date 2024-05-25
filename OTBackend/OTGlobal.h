#ifndef OTGLOBAL_H
#define OTGLOBAL_H

#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QDate>
#include <QApplication>
#include <QProcess>
#include <QFileDialog>
#include <QtNetwork>
#include <QDesktopServices>
#include <QScreen>
#include <QClipboard>
#include <QStyleFactory>

class OTFileMethods
{
public:
    enum methods
    {
        open = 1,
        reOpen = 2,
        silentOpen = 3,
        save = 4,
        saveAs = 5,
        backupSave = 6
    };
    Q_DECLARE_FLAGS(fileMethods, methods)
};

class OTBuildOptions
{
public:
    enum methods
    {
        Dev = 1,
        Alpha = 2,
        Beta = 3,
        Prerelease = 6,
        Release = 7

        /* Do not use:
         * EA = 4
         * Lite = 5
         */
    };
    Q_DECLARE_FLAGS(buildOptions, methods)

    static QString getBuildName(buildOptions flag)
    {
        switch (flag)
        {
        case Dev:           return "Dev";           break;
        case Alpha:         return "Alpha";         break;
        case Beta:          return "Beta";          break;
        case Prerelease:    return "Prerelease";    break;
        case Release:       return "Release";       break;
        default:            return "undefined";     break;
        }

        /* Do not use:
         * case EA: return "EA"; break;
         * case Lite: return "Lite"; break;
         */
    }
};

/// Unsaved code snippet for closeEvents:
/*
    if (unsaved)
    {
        int msgResult = msg.unsavedContent(this);
        if (msgResult == -1)
        {
            event->ignore();
            return;
        }
        else if (msgResult == 1)
            save(OTFileMethods::save, font.path);
    }
*/

/// [STATIC] General application information
class OTInformation
{
public:
    inline static const QString name = "OMSI-Tools";
    inline static const OTBuildOptions::buildOptions build = OTBuildOptions::Beta;
    inline static const QString sourceCodeLength = "34.905";

    class versions
    {
    public:
        inline static const QList<QPair<QString, unsigned int>> allVersions = {
            //QPair<QString, unsigned int>("1.2.0", 40), // without "lite"!
            //QPair<QString, unsigned int>("1.2.0-beta", 39),
            //QPair<QString, unsigned int>("1.2.0-dev", 38),
            //QPair<QString, unsigned int>("1.1.2-lite", 37),
            QPair<QString, unsigned int>("1.1.2-beta", 36),
            QPair<QString, unsigned int>("1.1.2-dev", 35),
            QPair<QString, unsigned int>("1.1.1-lite", 34),
            QPair<QString, unsigned int>("1.1.0-lite", 33),
            QPair<QString, unsigned int>("1.1.0-beta", 32),
            QPair<QString, unsigned int>("1.1.0-dev", 31),
            QPair<QString, unsigned int>("1.0.0-lite", 30),
            QPair<QString, unsigned int>("1.0.0-beta", 29),
            QPair<QString, unsigned int>("1.0.0-dev", 28),
            QPair<QString, unsigned int>("0.10.1-lite", 27),
            QPair<QString, unsigned int>("0.10.1-beta", 26),
            QPair<QString, unsigned int>("0.10.1-dev", 25),
            QPair<QString, unsigned int>("0.10.0-lite", 24),
            QPair<QString, unsigned int>("0.10.0-prerelease", 23),
            QPair<QString, unsigned int>("0.10.0-beta", 22),
            QPair<QString, unsigned int>("0.10.0-dev", 21),
            QPair<QString, unsigned int>("0.9.0-beta", 20),
            QPair<QString, unsigned int>("0.9.0-dev", 19),
            QPair<QString, unsigned int>("0.8.0-beta", 18),
            QPair<QString, unsigned int>("0.8.0-dev", 17),
            QPair<QString, unsigned int>("0.7.0-beta", 16),
            QPair<QString, unsigned int>("0.7.0-dev", 15),
            QPair<QString, unsigned int>("0.6.0-beta", 14),
            QPair<QString, unsigned int>("0.6.0-dev", 13),
            QPair<QString, unsigned int>("0.5.0-beta", 12),
            QPair<QString, unsigned int>("0.5.0-dev", 11),
            QPair<QString, unsigned int>("0.4.0-beta", 10),
            QPair<QString, unsigned int>("0.4.0-dev", 9),
            QPair<QString, unsigned int>("0.3.0-beta", 8),
            QPair<QString, unsigned int>("0.3.0-dev", 7),
            QPair<QString, unsigned int>("0.2.1-beta", 6),
            QPair<QString, unsigned int>("0.2.1-dev", 5),
            QPair<QString, unsigned int>("0.2.0-beta", 4),
            QPair<QString, unsigned int>("0.2.0-dev", 3),
            QPair<QString, unsigned int>("0.1.0-beta", 2),
            QPair<QString, unsigned int>("0.1.0-dev", 1),
            QPair<QString, unsigned int>("0.0.0-dev", 0)
        };

        inline static const QPair<QString, unsigned int> currentVersion = allVersions.at(0);
    };
};

/// [STATIC] Collection of links
class OTLinks
{
public:
    inline static const QString showroom = "https://reboot.omsi-webdisk.de/community/thread/4783";
    inline static const QString support = "https://reboot.omsi-webdisk.de/community/thread/5683";
    inline static const QString feedbackForm = "https://forms.gle/5VGy7fwohHys18bf9";
    inline static const QString github = "https://github.com/pietr26/OMSI-Tools";
    inline static const QString roadmap = "https://trello.com/b/wlId7JqM/roadmap-omsi-tools";

    inline static const QString apiPath = "https://backend.omsi-tools.de/api/v2/";
    inline static const QString latestVersion = apiPath + "?ref=currentVersion";
    inline static const QString releaseNotes = apiPath + "?ref=releaseNotes";
    inline static const QString versionBranches = apiPath + "?ref=versionBranches";
    inline static const QString inAppMessages = apiPath + "?ref=messages";
    inline static const QString download = apiPath + "?ref=downloadLink";

    inline static const QString csSuggestFiles = "https://backend.omsi-tools.de/api/csSuggestFiles/";
    inline static const QString csReportLink = "https://backend.omsi-tools.de/api/csReportLink/";
    inline static const QString empty = "https://backend.omsi-tools.de/api/empty/";

    inline static const QString bugDocUploads = "https://media.omsi-tools.de/bugDocUploads/";

    class Wiki
    {
    public:
        inline static const QString general = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Hauptseite";
        inline static const QString fonts = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Schriften";
        inline static const QString cleanup = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Aufräumer";
        inline static const QString contentSearch = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Inhaltssuche";
        inline static const QString verifyMap = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Kartenüberprüfung";
        inline static const QString preferences = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Einstellungen";
        inline static const QString maps = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Kartenbearbeitung";
        inline static const QString money = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Währungen";
        inline static const QString tickets = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Fahrkarten";
        inline static const QString lfClient = "https://wiki.omsi-tools.de/Spezial:Meine_Sprache/Leistellen-Client";
    };

    class WDT
    {
    public:
        inline static const QString blackModList = "https://reboot.omsi-webdisk.de/wiki/entry/140-moderation-allgemeines/#3-Schwarze-Liste-der-Modifikationen";
        inline static const QString workflowFilebase = "https://reboot.omsi-webdisk.de/wiki/entry/143-moderation-arbeit-in-der-filebase/#2-Datei-Eintrge";
    };

    class lfClient
    {
    public:
        inline static const QString apiPath = "https://backend.omsi-tools.de/api/lfClient/v1/";
        inline static const QString getSpeakRequests = apiPath + "getAllSpeakRequests.php";
        inline static const QString getParticipant = apiPath + "getParticipantData.php";
        inline static const QString getParticipants = apiPath + "getAllParticipants.php";
        inline static const QString setSpeakRequest = apiPath + "requestSpeak.php";
    };
};

class OTNetworkConnection : public QObject
{
    Q_OBJECT
public slots:
    /// [OVERLOADED] POST with file / Returns the downloaded file
    QByteArray post(const QUrl &url, QFile *file, QList<QPair<QString, QString>> params = QList<QPair<QString, QString>>(), unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        return downloadPostFile(url, params, file, connectionTimeout);
    }

    /// [OVERLOADED] POST / Returns the downloaded file
    QByteArray post(const QUrl &url, QList<QPair<QString, QString>> params = QList<QPair<QString, QString>>(), unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        return downloadPost(url, params, connectionTimeout);
    }

    /// [OVERLOADED] POST / Saves the download file to a local file
    int post(const QUrl &url, const QString filepath, QList<QPair<QString, QString>> params = QList<QPair<QString, QString>>(), unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        saveToFile(filepath, downloadPost(url, params, connectionTimeout));
        return lastHttpCode;
    }

    /// [OVERLOADED] GET / Returns the downloaded file
    QByteArray get(const QUrl &url, unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        return downloadGet(url, connectionTimeout);
    }

    /// [OVERLOADED] GET / Saves the download file to a local file
    int get(const QUrl &url, const QString filepath, unsigned int connectionTimeout = 10000)
    {
        lastSuccess = 0;
        saveToFile(filepath, downloadGet(url, connectionTimeout));
        return lastHttpCode;
    }

public:
    QNetworkReply *reply;
    int currentProgress;
    int maxProgress;
    int lastHttpCode;

    /// Check if the last connection was successfully. -2: HTTP 503 | -1: No | 0: NULL | 1: Yes
    int lastSuccess;

private slots:
    void downloadProgress(qint64 current, qint64 max)
    {
        currentProgress = current;
        maxProgress = max;
    }

private:
    QNetworkAccessManager manager;

    /// Main part of downloading a file
    QByteArray downloadPost(const QUrl &url, QList<QPair<QString, QString>> params, unsigned int connectionTimeout)
    {
        qDebug().noquote().nospace() << "POST to '" << url.url() << "'";
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setTransferTimeout(connectionTimeout);

        QListIterator<QPair<QString, QString>> paramIterator(params);
        qDebug().noquote() << "URL params:" << params;

        QUrlQuery urlParams;

        while (paramIterator.hasNext())
        {
            QPair<QString, QString> param = paramIterator.next();
            urlParams.addQueryItem(param.first, param.second);
        }

        QEventLoop loop;

        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = manager.post(request, urlParams.toString(QUrl::FullyEncoded).toUtf8());

        connect(reply, &QNetworkReply::downloadProgress, this, &OTNetworkConnection::downloadProgress);
        loop.exec();

        // Content length:
        // reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastHttpCode = httpCode;

        qDebug().noquote() << QString("POST finished (HTTP %1)").arg(httpCode);

        if (url.url().contains("omsi-tools.de") && (httpCode == 503))
        {
            lastSuccess = -2;
            qWarning().noquote() << QString("%1 is currently undergoing maintenance (HTTP 503). Please try again later.").arg(url.host());
        }
        else if ((httpCode >= 300) || (httpCode == 0))
        {
            lastSuccess = -1;
            return "";
        }
        else
            lastSuccess = 1;

        return reply->readAll();
    }

    /// Main part of downloading a file
    QByteArray downloadPostFile(const QUrl &url, QList<QPair<QString, QString>> params, QFile *file, unsigned int connectionTimeout)
    {
        qDebug().noquote().nospace() << "POST to '" << url.url() << "'";
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
        request.setTransferTimeout(connectionTimeout);

        QListIterator<QPair<QString, QString>> paramIterator(params);
        qDebug().noquote() << "URL params:" << params;

        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart imagePart;
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\"; filename=\"" + QFileInfo(*file).fileName() + "\""));
        file->open(QFile::ReadOnly);
        imagePart.setBodyDevice(file);
        file->setParent(multiPart); // ownership transferred
        multiPart->append(imagePart);

        QEventLoop loop;

        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = manager.post(request, multiPart);

        connect(reply, &QNetworkReply::downloadProgress, this, &OTNetworkConnection::downloadProgress);
        loop.exec();

        // Content length:
        // reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastHttpCode = httpCode;

        qDebug().noquote() << QString("POST finished (HTTP %1)").arg(httpCode);

        if (url.url().contains("omsi-tools.de") && (httpCode == 503))
        {
            lastSuccess = -2;
            qWarning().noquote() << QString("%1 is currently undergoing maintenance (HTTP 503). Please try again later.").arg(url.host());
        }
        else if ((httpCode >= 300) || (httpCode == 0))
        {
            lastSuccess = -1;
            return "";
        }
        else
            lastSuccess = 1;

        return reply->readAll();
    }

    /// Main part of downloading a file
    QByteArray downloadGet(const QUrl &url, unsigned int connectionTimeout)
    {
        qDebug().noquote().nospace() << "GET to '" << url.url() << "'";
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setTransferTimeout(connectionTimeout);

        QEventLoop loop;

        connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
        reply = manager.get(request);

        connect(reply, &QNetworkReply::downloadProgress, this, &OTNetworkConnection::downloadProgress);
        loop.exec();

        // Content length:
        // reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        lastHttpCode = httpCode;

        if (url.url().contains("omsi-tools.de") && (httpCode == 503))
        {
            lastSuccess = -2;
            qWarning().noquote() << QString("%1 is currently undergoing maintenance (HTTP 503). Please try again later.").arg(url.host());
        }
        else if ((httpCode >= 300) || (httpCode == 0))
        {
            lastSuccess = -1;
            return "";
        }
        else
            lastSuccess = 1;

        return reply->readAll();
    }

    /// Saves the download file to a local file
    void saveToFile(const QString filepath, const QByteArray content)
    {
        QFile file(filepath);
        if (!file.open(QFile::WriteOnly))
            qWarning().noquote() << "Error while saving POST content!";
        else
            file.write(content);
    }
};

/// Class for miscellaneous
class OTMiscellaneous
{
public:
    /// Opens a file in explorer
    void openInExplorer(QString path)
    {
        QStringList args;
        args << "/select," << QDir::toNativeSeparators(path);

        QProcess *process = new QProcess();
        process->start("explorer.exe", args);
    }

    /// Restarts the application
    void restart()
    {
        qInfo() << "Restart application...";
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }

    /// Returns curreent time
    QString getTime(QString format = "hh:mm:ss")
    {
        return QTime::currentTime().toString(format);
    }

    /// Returns current date
    QString getDate(QString format = "dd.MM.yyyy")
    {
        return QDate::currentDate().toString(format);
    }

    /// Sizes the window in dependence to the screen geometry
    QSize sizeWindow(double width, double height)
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        QSize windowSize = screen->availableGeometry().size();
        windowSize.setWidth(windowSize.width() * width);
        windowSize.setHeight(windowSize.height() * height);

        return windowSize;
    }

    /// Returns data to move a widget to the screen center
    QPoint centerPosition(QWidget* parent)
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        return screen->geometry().center() - parent->rect().center();
    }

    /// Puts a string to clipboard
    void copy(QString copytext)
    {
        qDebug() << "Copy string...";
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copytext);
    }

    void copyPixmap(QPixmap copyPixmap)
    {
        qDebug() << "Copy image...";
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setPixmap(copyPixmap);
    }

    /// Checks for an update - index 0: "503": maintenance, "false" = error, "noUpdates" = no updates available, else: new version | index 1: latestVersion
    QStringList getUpdateInformation()
    {

        qDebug() << "Check for updates...";
        OTNetworkConnection nc;
        const QString latestVersion = nc.post(OTLinks::latestVersion);
        QStringList list;

        if (nc.lastSuccess == -2)
            list << "503";
        else if (latestVersion == "")
            list << "false";
        else if (latestVersion == OTInformation::versions::currentVersion.first)
            list << "noUpdates";
        else
            list << latestVersion;

        list << latestVersion;
        return list;
    }

    /// Searches for updates and returns information to the user
    void startUpdate(QWidget *parent, bool clearAppDir)
    {
        qDebug() << "Create tempAppDir and copy files...";

        QDirIterator dirIterator(QApplication::applicationDirPath(), QStringList{"*.*"}, QDir::Files, QDirIterator::Subdirectories);
        while (dirIterator.hasNext())
        {
            QString file = dirIterator.next();

            QDir().mkdir(QDir::tempPath() + "/OMSI-Tools_tempAppDir");

            QString newFilePath = QDir::tempPath() + "/OMSI-Tools_tempAppDir/" + file.remove(0, QApplication::applicationDirPath().size() + 1);
            QDir().mkpath(QFileInfo(newFilePath).absolutePath());

            qDebug().noquote() << "New path:" << newFilePath;
            if (QFile(newFilePath).exists())
                QFile(newFilePath).remove();

            qDebug().noquote() << "From path:" << QApplication::applicationDirPath() + "/" + file;
            qDebug() << "-----------------------------------------------------------------";
            QFile(QApplication::applicationDirPath() + "/" + file).copy(newFilePath);
        }
        qDebug() << "Finished.";

        QStringList args;
        args << "callFromMainApplication";
        args << QApplication::applicationDirPath();

        if (clearAppDir)
            args << "clearAppDir";

        if (!QProcess::startDetached(QDir::tempPath() + "/OMSI-Tools_tempAppDir/OMSI-Tools_Updater.exe", args))
        {
            qWarning() << "There was an error while starting the updater (with name 'OMSI-Tools_Updater.exe').";
            QMessageBox::warning(parent, QObject::tr("Updating %1").arg(OTInformation::name), QObject::tr("There was an error while starting the updater. Please retry it or contact the developer."));
        }
        else
        {
            qInfo() << "Start Updater...";
            QApplication::quit();
        }
    }
};

/// Class for file operations
class OTFileOperations
{
public:
    /// Returns an universal file header
    QString writeFileHeader()
    {
        return "File created with " + OTInformation::name + " " + OTInformation::versions::currentVersion.first + " on " + misc.getDate() + ", " + misc.getTime() + "\n\n";
    }

    /// Checks if the backups folder exists
    void createBackupFolder()
    {
        if (!QDir("backup").exists())
            QDir().mkdir("backup");
    }

    /// Creates a shortcut
    void createShortcut(QString filepath, QString shortcutLocation, QWidget *parent)
    {
        if (QFile(filepath).link(shortcutLocation))
        {
            QMessageBox::information(parent, QObject::tr("Success"), QObject::tr("Successfully created shortcut!"));
            qInfo().noquote() << QString("Created shortcut in '%1'!").arg(shortcutLocation);
        }
        else
        {
            QMessageBox::warning(parent, QString(QObject::tr("Error")), QObject::tr("Could not create shortcut in %1.").arg(QDir().homePath() + "/Desktop"));
            qCritical().noquote() << QString("Could not create a shortcut in '%1'!").arg(shortcutLocation);
        }
    }

    /// Selects a folder or file in explorer
    void showInExplorer(QString absolutePath)
    {
        // ATTENTION: This will NOT work in the OneDrive folder - I don't know why.
        QStringList args;
        args << "/select," << QDir::toNativeSeparators(absolutePath);
        qDebug() << "Show in explorer:" << absolutePath;

        QProcess *process = new QProcess();
        process->start("explorer.exe", args);
    }

private:
    OTMiscellaneous misc;
};

/// Setting reader, writer etc.
class OTSettings
{
public:
    /// Writes a setting
    void write(QString module, QString name, QVariant value)
    {
        QSettings preferences(OTInformation::name, module);
        preferences.setValue(name, value);
        qDebug().noquote().nospace() << "Write pref to " << module << ": "<< name << ", value: " << value;
    }

    /// Reads a setting
    QVariant read(QString module, QString name, bool logging = true)
    {
        QSettings preferences(OTInformation::name, module);
        QVariant value = preferences.value(name);
        if (logging) qDebug().noquote().nospace() << "Read pref from " << module << ": "<< name << ", value: " << value;

        return value;
    }

    void remove(QString module, QString name)
    {
        QSettings(OTInformation::name, module).remove(name);
    }

    void removeAll()
    {
        QSettings("HKEY_CURRENT_USER\\SOFTWARE\\" + OTInformation::name, QSettings::NativeFormat).remove("");
    }

    /// Checks if OMSI main dir exists / is valid
    bool checkMainDir(QWidget *parent, QString mainDir, bool openMessage)
    {
        if (mainDir != "" && !QFileInfo(QFile(mainDir + "/Omsi.exe")).exists())
        {
            qWarning().noquote() << "'" + mainDir + "' isn't an OMSI path!";

            if (openMessage)
            {
                QMessageBox::StandardButton reply = QMessageBox::warning(parent, QObject::tr("Could not found \"Omsi.exe\""), QObject::tr("'Omsi.exe' could not found in the selected directory. Is it the correct path? Otherwise, problems may appear in some modules. Should a new path be selected?"), QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes)
                    getOmsiPath(parent);
            }

            return false;
        }

        return true;
    }

    /// Select and control OMSI main dir path. Returns the OMSI path.
    QString getOmsiPath(QWidget *parent, bool openMessage = true, QString path = "")
    {
        if (path.isEmpty())
            path = QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\aerosoft\\OMSI 2", QSettings::NativeFormat).value("Product_Path").toString();

        QString mainDir = QFileDialog::getExistingDirectory(parent, QObject::tr("Select the OMSI main directory..."), path);

        if (openMessage) checkMainDir(parent, mainDir, openMessage);

        return mainDir;
    }

    /// Gets all prefs keys and its values
    QList<QPair<QString, QVariant>> getAllPreferences()
    {
        QSettings set("HKEY_CURRENT_USER\\SOFTWARE\\" + OTInformation::name, QSettings::NativeFormat);

        QList<QPair<QString, QVariant>> settings;
        QStringList keys = set.allKeys();

        foreach (QString current, keys)
            settings.append(QPair<QString, QVariant>(current, set.value(current)));

        return settings;
    }

    /// Gets all prefs keys and its values in a console formatting
    QString getAllPreferencesFormatted()
    {
        QSettings set("HKEY_CURRENT_USER\\SOFTWARE\\" + OTInformation::name, QSettings::NativeFormat);

        QString returnString = "\n";

        foreach (QString current, set.allKeys())
            returnString += current + " -> " + set.value(current).toString() + "\n";

        return returnString;
    }

    /// Installs a translator
    void loadTranslator()
    {
        QTranslator *translator = new QTranslator();
        QTranslator *baseTranslator = new QTranslator();

        QString languageFile;
        QString baseLanguageFile;
        switch (read("main", "language").toInt())
        {
            //case 0: english
            case 1: languageFile = "tr_de"; baseLanguageFile = "qtbase_de"; break;
            case 2: languageFile = "tr_fr"; baseLanguageFile = "qtbase_fr"; break;
            case 3: languageFile = "tr_it"; baseLanguageFile = "qtbase_it"; break;
            case 4: languageFile = "tr_cs"; baseLanguageFile = "qtbase_cs"; break;

            case 5: languageFile = "tr_et"; baseLanguageFile = "qtbase_en"; break;
            case 6: languageFile = "tr_eo"; baseLanguageFile = "qtbase_en"; break;
            case 7: languageFile = "tr_fi"; baseLanguageFile = "qtbase_fi"; break;
            case 8: languageFile = "tr_is"; baseLanguageFile = "qtbase_en"; break;
            case 9: languageFile = "tr_ja"; baseLanguageFile = "qtbase_ja"; break;
            case 10: languageFile = "tr_cy"; baseLanguageFile = "qtbase_en"; break;
        }

        if (languageFile != "")
        {
            bool trLoad = translator->load(languageFile, ":/rec/data/translations/");
            bool trInstall = qApp->installTranslator(translator);

            bool baseTrLoad = baseTranslator->load(baseLanguageFile, ":/rec/data/translations/");
            bool baseTrInstall = qApp->installTranslator(baseTranslator);

            if (trLoad && trInstall && baseTrLoad && baseTrInstall)
                qInfo() << "Loaded translations";
            else
            {
                qWarning() << "Error while loading translations!";

                qDebug().noquote() << "trLoad:" << trLoad;
                qDebug().noquote() << "trInstall:" << trInstall;
                qDebug().noquote() << "baseTrLoad:" << baseTrLoad;
                qDebug().noquote() << "baseTrInstall:" << baseTrInstall;
            }
        }
    }

    /// Sets default needed preferences
    void setDefaultPreferences()
    {
        if (!read("main", "autoUpdateCheck").isValid())
            write("main", "autoUpdateCheck", 2);

        if (!read("main", "logfileMode").isValid())
            write("main", "logfileMode", 0);

        if (!read("main", "autosave").isValid())
            write("main", "autosave", true);

        bool themeOk = false;
        read("main", "theme").toString().toInt(&themeOk);

        if (!read("main", "theme").isValid() || themeOk)
            write("main", "theme", "Fusion");

        if (!read("main", "autosaveDuration").isValid())
            write("main", "autosaveDuration", 60);

        if (!read("wStart", "messagesVisible").isValid())
            write("wStart", "messagesVisible", true);

        if (!read("wFonts", "texPreview").isValid())
            write("wFonts", "texPreview", 1);

        if (!QFile("texconv.exe").exists()) qInfo() << "Extract textconv.exe:" << QFile().copy(":/rec/data/external/texconv.exe", "texconv.exe");

        if (!QFile("_docs/Handbuch DE.pdf").exists())
        {
            QDir().mkdir("_docs");
            qInfo() << "Extract manuals:" << QFile().copy(":/rec/data/manual/Handbuch DE.pdf", "_docs/Handbuch DE.pdf");
        }
    }

    QString getCurrentLanguageCode()
    {
        int index = read("main", "language").toInt();

        switch (index) {
            case 0: return "en"; break;
            case 1: return "de"; break;
            case 2: return "fr"; break;
            case 3: return "it"; break;
            case 4: return "cz"; break;
            default: return "err";
        }
    }

    bool devModeEnabled()
    {
        if (OTInformation::build != OTBuildOptions::Dev) return read("main", "devToolsEnabled").toBool();

        return true;
    }
};

/// Message class
class OTMessage
{
public:
    void tempDBOpenError(QWidget *parent, QString errorTitle)
    {
        QMessageBox::critical(parent, errorTitle, QObject::tr("There was an error while open the temporary database. Please try it again or contect the developer."));
    }

    /// Asks if the user wants to set the main dir
    bool setMainDir(QWidget *parent)
    {
        qDebug() << "Message: Action needs main directory. Set now (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("OMSI main directory not found"), QObject::tr("To continue the application needs the OMSI main directory. Should it be done now?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return false;

        return true;
    }

    /// universal unsaved-Message (with Save, Discard and Cancel). Returns 'save = 1', 'discard = 0' or 'cancel = -1'
    int unsavedChanges(QWidget *parent)
    {
        qDebug() << "Message: Save unsaved changes (Save/Discard/Cancel)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Unsaved changes"), QObject::tr("There are unsaved changes."), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (reply == QMessageBox::Save)
        {
            qDebug() << "User pressed Save";
            return 1;
        }
        else if (reply == QMessageBox::Discard)
        {
            qDebug() << "User pressed Discard";
            return 0;
        }
        else
        {
            qDebug() << "User pressed Cancel";
            return -1;
        }
    }

    /// universal unsaved-Message (with Yes and No). Returns 'Yes = 1' or 'No = 0'
    bool unsavedContentLeaveYesNo(QWidget *parent)
    {
        qDebug() << "Message: Save unsaved changes / leave (Yes/No)?";
        QMessageBox::StandardButton reply = QMessageBox::question(parent, QObject::tr("Unsaved changes"), QObject::tr("There are unsaved changes. Close anyway?"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            qDebug() << "User pressed Yes";
            return true;
        }
        else
        {
            qDebug() << "User pressed No";
            return false;
        }
    }

    /// If the user wants to open a file via a button and the file doesn't exist, this message es helpful
    void fileOpenError(QWidget *parent)
    {
        QMessageBox::information(parent, QObject::tr("Error while opening file"), QObject::tr("The selected file doesn't exists or is read-protected."));
    }

    /// Could not open a file from OMSI main dir
    void fileOpenErrorCloseOMSI(QWidget *parent, QString filename)
    {
        QMessageBox::critical(parent, QObject::tr("Error while opening file"), QString(QObject::tr("There was an error while opening '%1'. If OMSI is running, please close it and retry it. Furthermore, check if the file still exists.")).arg(filename));
    }

    /// Font module: No chars in font
    void noCharsInFont(QWidget *parent)
    {
        QMessageBox::warning(parent, QObject::tr("No chars in font"), QObject::tr("There are no chars in the font."));
    }

    /// There was en error while saving a file
    void fileSaveError(QString filename, QWidget *parent = 0)
    {
        QMessageBox::warning(parent, QObject::tr("Save error"), QString(QObject::tr("There was an save error '%1'. If OMSI is running, please close it and retry it.")).arg(filename));
    }

    /// A module is due to narrow time deactivated
    void moduleDisabled(QWidget *parent = 0)
    {
        QMessageBox::information(parent, QObject::tr("Module still deactivated"), QObject::tr("Unfortunately, this module is still deactivated due to a time shortage in the developments. Please check for new updates in the next few days."));
    }
};

/// Calculates disk usage
class OTDiskUsage
{
public:
    /// Returns disk usage of directory 'dirPath'
    QString formatSize(QString dirPath)
    {
        qint64 size = calcDirSize(dirPath);

        int i;
        double outputSize = size;
        for(i = 0; i < units.size() - 1; i++)
        {
            if (outputSize < 1024)
                break;

            outputSize = outputSize / 1024;
        }

        return QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);;
    }

    /// Formats bytes values into other units if necessary
    QString calculateSize(qint64 size)
    {
        int i;
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
    /// Calculates size in bytes of a directory
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

/// [STATIC] Class for some global needed strings
class OTStrings
{
public:
    // Multilanguage strings
    static inline const QString langCs = "čeština (Czech)";
    static inline const QString langEn = "English";
    static inline const QString langFi = "suomi (Finnish)";
    static inline const QString langFr = "français (French)";
    static inline const QString langDe = "Deutsch (German)";
    static inline const QString langIt = "Italiano (Italian)";
    static inline const QString langJa = "日本語 (Japanese)";

    static inline const QString textureSuffixes = QObject::tr("Images") + " (*.png *.jpg *.jpeg *.bmp *.dds)";

    /// Fun facts (funFact, time in ms)
    static QList<QPair<QString, unsigned int>> getFunFacts()
    {
        OTSettings set;

        return QList<QPair<QString, unsigned int>> {
            // Link HTML: <a style='color: lightblue' href='LINK'>TEXT</a>
            QPair<QString, unsigned int>(QObject::tr("The source code of %1 is about %2 lines long.").arg(OTInformation::name, OTInformation::sourceCodeLength), 8000),
            QPair<QString, unsigned int>(QObject::tr("%1 was born from a simple console application called \"Font Creator\".").arg(OTInformation::name), 10000),
            QPair<QString, unsigned int>(QObject::tr("With the updater of %1 you can install updates with one click. It is available through the preferences.").arg(OTInformation::name), 13000),
            QPair<QString, unsigned int>(QObject::tr("In the <a style='color: lightblue' href='%2'>Wiki of %1</a> you can find useful explanations about all topics.", "Copy whole source text to prevent translations faults in HTML code").arg(OTInformation::name, OTLinks::Wiki::general), 11000),
            QPair<QString, unsigned int>(QObject::tr("Check out the latest developments in the <a style='color: lightblue' href='%1'>presentation thread in the OMSI-WebDisk</a>.", "Copy whole source text to prevent translations faults in HTML code").arg(OTLinks::showroom), 11000),
            QPair<QString, unsigned int>(QObject::tr("Your hard disk is crowded? Clean up your main directory with %1' cleanup tool.").arg(OTInformation::name), 10000),
            QPair<QString, unsigned int>(QObject::tr("You've started %1 about %2 times.").arg(OTInformation::name, set.read("main", "startCount").toString()), 8000)
        };
    };

    static QString serverMaintenance() { return QObject::tr("The application server is currently undergoing maintenance (HTTP 503). Please try again later."); }

    static QList<QPair<QString, QString>> getThemeNames()
    {
        QStringList styles = QStyleFactory::keys();
        QList<QPair<QString, QString>> themes;

        for (int i = 0; i < styles.count(); i++)
        {
            themes.append(QPair<QString, QString>(styles[i], ""));
            if (styles[i] == "windows11")
            {
                themes[i].second = "Windows 11";
            }
            else if (styles[i] == "windowsvista")
            {
                themes[i].second = QObject::tr("Windows standard");
            }
            else if (styles[i] == "Windows")
            {
                themes[i].second = "Windows XP";
            }
            else if (styles[i] == "Fusion")
            {
                themes[i].second = "Fusion";
            }
            else
            {
                themes[i].second = themes[i].first;
            }
        }

        return themes;
    }
};

class OTInAppMessage
{
public:
    QString ID;
    int publicity;
    QDateTime start;
    QDateTime end;
    QString slug;
    QStringList versions;

    QString enTitle;
    QString enShortDescription;
    QString enDescription;

    QString deTitle;
    QString deShortDescription;
    QString deDescription;

    QString image;

    int trashbin;
    QDateTime deletedAt;
};

#endif // OTGLOBAL_H
