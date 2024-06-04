#ifndef LFCLIENTAPIINTERFACE_H
#define LFCLIENTAPIINTERFACE_H

#include <QObject>

#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "OTBackend/OTGlobal.h"

class LFCApiSingleNotification
{
public:
    LFCApiSingleNotification() { }
    QStringList lines;
    QString title;
    QString text;
};

class LFCApiNotification
{
public:
    explicit LFCApiNotification(const QJsonObject &obj = {})
    {
        QJsonArray affectedLines = obj.value("affected_lines").toArray();

        _id            = obj.value("id").toInt();
        _enabled       = obj.value("enabled").toBool();
        for(int i = 0; i < affectedLines.count(); i++)
            _affectedLines << affectedLines.at(i).toString();
        _title         = obj.value("title").toString();
        _text          = obj.value("text").toString();
    }

    int         id()             const { return _id; }
    bool        isEnabled()      const { return _enabled; }
    QStringList afftectedLines() const { return _affectedLines; }
    QString     title()          const { return _title; }
    QString     text()           const { return _text; }

private:
    int _id;
    bool _enabled;
    QStringList _affectedLines;
    QString _title, _text;
};

class LFCApiBusstop
{
public:
    explicit LFCApiBusstop(int delay, QString name, int status, QString time, int notificationID)
    {
        _delay = delay;
        _name = name;

        switch (status)
        {
            case -1: _status = Status::canceled;                break;
            case 0:  _status = Status::notAssigned;             break;
            case 1:  _status = Status::approvedForSubstitution; break;
            case 2:  _status = Status::runsAsPlanned;           break;
        }

        _time = QTime::fromString(time, "HH:mm:ss");
        _notificationID = notificationID;
    }

    enum Status
    {
        canceled = -1,
        notAssigned = 0,
        approvedForSubstitution = 1,
        runsAsPlanned = 2
    };

    int delay() const { return _delay; }
    QString name() const { return _name; }
    Status status() const { return _status; }
    QTime time() const { return _time; }
    int notificationID() const { return _notificationID; }

private:
    int _delay;
    QString _name;
    Status _status;
    QTime _time;
    int _notificationID;
};

class LFCApiTrip
{
public:
    explicit LFCApiTrip(const QJsonObject &obj = {})
    {
        QJsonArray busstops = obj.value("busstops").toArray();

        _assignee = obj.value("assignee").toString();
        for(int i = 0; i < busstops.count(); i++)
            _busstops << LFCApiBusstop(busstops[i].toObject().value("delay").toInt(), busstops[i].toObject().value("name").toString(), busstops[i].toObject().value("status").toInt(), busstops[i].toObject().value("time").toString(), busstops[i].toObject().value("notification_ID").toInt());

        _id = obj.value("id").toInt();
        _line = obj.value("line").toInt();
        _name = obj.value("name").toString();
        _route = obj.value("route").toInt();
        _vehicle = obj.value("vehicle").toInt();
    }

    QString assignee() const { return _assignee; }
    QList<LFCApiBusstop> busstops() const { return _busstops; }
    int id() const { return _id; }
    int line() const { return _line; }
    QString name() const { return _name; }
    int route() const { return _route; }
    int vehicle() const { return _vehicle; }

private:
    QString _assignee;
    QList<LFCApiBusstop> _busstops;
    int _id;
    int _line;
    QString _name;
    int _route;
    int _vehicle;
};

class LFCApiGlobalData {
public:
    explicit LFCApiGlobalData(const QJsonObject &obj = {}) {
        _mapName =   obj.value("map").toString();
        _timeDiff =  obj.value("time_diff").toInt();
        _lockTrips = obj.value("lock_trips").toBool();
    }

    QString mapName() const { return _mapName; }
    int timeDiff()    const { return _timeDiff; }
    bool lockTrips()  const { return _lockTrips; }

private:
    QString _mapName;
    int _timeDiff;
    bool _lockTrips;
};

class LFCApiVehicle {
public:
    explicit LFCApiVehicle(const QJsonObject &obj = {}) {
        _id = obj.value("id").toInt();
        _number = obj.value("number").toString();
        _name = obj.value("name").toString();
    }

    int id() const { return _id; }

    QString number() const { return _number; }

    QString name() const { return _name; }

private:
    int _id;
    QString _number;
    QString _name;
};

enum LFCApiSpeakRequest
{
    none = 0,
    speak = 1,
    crash = 2,
    talking = 3
};

class LFClientAPIInterface : public QObject
{
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
        UpdateUserStatus,
        GetTrips,
        GetMyTrips,
        GetVehicle,
        GetAllVehicles,
        GetMySpeakRequest
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
    QList<LFCApiTrip> getMyTrips();
    LFCApiVehicle getVehicle(const int &id);
    QList<LFCApiVehicle> getAllVehicles();
    LFCApiSpeakRequest getMySpeakRequest();
    void requestSpeak(LFCApiSpeakRequest request);

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
