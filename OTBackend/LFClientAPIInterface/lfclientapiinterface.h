#ifndef LFCLIENTAPIINTERFACE_H
#define LFCLIENTAPIINTERFACE_H

#include <QObject>

#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkAccessManager>
#include <QNetworkReply>

class LFCApiSingleNotification {
#include "OTBackend/OTGlobal.h"
public:
    LFCApiSingleNotification() { }
    QStringList lines;
    QString title;
    QString text;
};

class LFCApiNotification {
public:
    explicit LFCApiNotification(const QJsonObject &obj = {}) {
        QJsonArray affectedLines = obj.value("affected_lines").toArray();

        _id            = obj.value("id").toInt();
        _enabled       = obj.value("enabled").toBool();
        for(int i = 0; i < affectedLines.count(); i++)
            _affectedLines << affectedLines.at(i).toString();
        _title         = obj.value("title").toString();
        _text          = obj.value("text").toString();
    }

    int         id()             const {return _id;}
    bool        isEnabled()      const {return _enabled;}
    QStringList afftectedLines() const {return _affectedLines;}
    QString     title()          const {return _title;}
    QString     text()           const {return _text;}

private:
    int _id;
    bool _enabled;
    QStringList _affectedLines;
    QString _title, _text;
};

class LFCApiGlobalData {
public:
    explicit LFCApiGlobalData(const QJsonObject &obj = {}) {
        _mapName =   obj.value("map").toString();
        _timeDiff =  obj.value("time_diff").toInt();
        _lockTrips = obj.value("lock_trips").toBool();
    }

    QString mapName() const {return _mapName;}
    int timeDiff()    const {return _timeDiff;}
    bool lockTrips()  const {return _lockTrips;}

private:
    QString _mapName;
    int _timeDiff;
    bool _lockTrips;
};

class LFClientAPIInterface : public QObject {
    Q_OBJECT
public:
    enum ErrorType {
        LoginInvalid,
        LoginUserNotFound,
        LoginWrongPassword,
        Unauthorized,
        AccessDenied,
        InvalidParams,
        SqlRequestError,
        UserNotFound,
        SpeakRequestAlreadyExists,
        SpeakRequestDoesNotExist
    };

    enum ApiEndpoint {
        AnswerSpeakRequest,
        GetAllParticipants,
        GetAllSpeakRequest,
        GetGlobalData,
        GetNotifications,
        GetParticipant,
        Login,
        Logout,
        RequestSpeak,
        UpdateUserStatus
    };

    explicit LFClientAPIInterface(QObject *parent);

    bool isLoggedIn();

    ErrorType errorType() const;
    QString errorString() const;
    QString currentToken() const;

    bool login(const QString &username, const QString &password);
    bool logout();

    LFCApiGlobalData getGlobalData();

    QList<LFCApiNotification> getNotifications();

    static inline const QString userNotFoundDescription  = QObject::tr("The specified user was not found.");
    static inline const QString wrongPasswordDescription = QObject::tr("The password is wrong.");

private:
    QNetworkRequest createNewRequest(const ApiEndpoint &, const QList<QPair<QString, QString>> &parameters = {}) const;
    QJsonObject handleReply(QNetworkReply *, bool *ok = nullptr, int *httpCode = nullptr);

signals:
    void loginStatusChanged(bool);

private:
    QNetworkAccessManager *m;
    QString _currentToken;
    QString _currentUserID;

    ErrorType _errorType;
    QString _errorString;
};

#endif // LFCLIENTAPIINTERFACE_H
