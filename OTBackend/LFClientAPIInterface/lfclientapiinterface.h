#ifndef LFCLIENTAPIINTERFACE_H
#define LFCLIENTAPIINTERFACE_H

#include <QObject>

#include <QJsonObject>

#include <QNetworkAccessManager>

class LfCApiGlobalData : public QObject {
    Q_OBJECT

public:
    explicit LfCApiGlobalData(QObject *parent, const QJsonObject &obj = {}) :
    QObject(parent) {
        _mapName =   obj.value("map").toString();
        _timeDiff =  obj.value("time_diff").toInt();
        _lockTrips = obj.value("lock_trips").toBool();
    }

    explicit LfCApiGlobalData(LfCApiGlobalData &other) {
        _mapName   = other.mapName();
        _timeDiff  = other.timeDiff();
        _lockTrips = other.lockTrips();
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

    LfCApiGlobalData getGlobalData();

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

    ErrorType _errorType;
    QString _errorString;
};

#endif // LFCLIENTAPIINTERFACE_H
