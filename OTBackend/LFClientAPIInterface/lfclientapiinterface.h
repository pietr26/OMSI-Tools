#ifndef LFCLIENTAPIINTERFACE_H
#define LFCLIENTAPIINTERFACE_H

#include <QObject>

#include <QNetworkAccessManager>

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

    ErrorType errorType() const;
    QString errorString() const;
    QString currentToken() const;

    bool login(const QString &username, const QString &password);
    bool logout();

    static inline const QString userNotFoundDescription  = QObject::tr("The specified user was not found.");
    static inline const QString wrongPasswordDescription = QObject::tr("The password is wrong.");

private:
    QNetworkRequest createNewRequest(const ApiEndpoint &, const QList<QPair<QString, QString>> &parameters = {}) const;
    QJsonObject handleReply(QNetworkReply *, bool *ok = nullptr, int *httpCode = nullptr);

signals:

private:
    QNetworkAccessManager *m;
    QString _currentToken;

    ErrorType _errorType;
    QString _errorString;
};

#endif // LFCLIENTAPIINTERFACE_H
