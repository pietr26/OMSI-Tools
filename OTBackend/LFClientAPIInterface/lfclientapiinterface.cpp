#include "lfclientapiinterface.h"

#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>


LFClientAPIInterface::LFClientAPIInterface(QObject *parent) :
    QObject(parent),
    m(new QNetworkAccessManager(this)) {
}

bool LFClientAPIInterface::isLoggedIn() {
    return !_currentToken.isEmpty();
}

LFClientAPIInterface::ErrorType LFClientAPIInterface::errorType() const {return _errorType;}
QString LFClientAPIInterface::errorString() const {return _errorString;}
QString LFClientAPIInterface::currentToken() const {return _currentToken;}

bool LFClientAPIInterface::login(const QString &username, const QString &password) {
    QNetworkRequest req = createNewRequest(Login);
    req.setRawHeader("Authorization", "Basic " + QString(username + ":" + password).toLocal8Bit().toBase64());
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    if(ok) {
        _currentToken = obj.value("token").toString();
        emit loginStatusChanged(true);
    }

    return ok;
}

bool LFClientAPIInterface::logout() {
    QNetworkRequest req = createNewRequest(Logout);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    if(ok) {
        _currentToken = "";
        emit loginStatusChanged(false);
    }
    return ok;
}

LFCApiGlobalData LFClientAPIInterface::getGlobalData() {
    QNetworkRequest req = createNewRequest(GetGlobalData);
    QNetworkReply *r = m->get(req);
    while(!r->isFinished())
        qApp->processEvents();

    bool ok;
    QJsonObject obj = handleReply(r, &ok);
    if(ok) {
        return LFCApiGlobalData(this, obj);
    } else
        return LFCApiGlobalData(this);
}

QNetworkRequest LFClientAPIInterface::createNewRequest(const ApiEndpoint &endpoint, const QList<QPair<QString, QString>> &parameters) const {
    QString url = "https://backend.omsi-tools.de/api/lfClient/v1/";
    switch(endpoint) {
        case AnswerSpeakRequest: url += "answerSpeakRequest.php";  break;
        case GetAllParticipants: url += "getAllParticipants.php";  break;
        case GetAllSpeakRequest: url += "getAllSpeakRequests.php"; break;
        case GetGlobalData:      url += "getGlobalData.php";       break;
        case GetParticipant:     url += "getParticipant.php";      break;
        case Login:              url += "login.php";               break;
        case Logout:             url += "logout.php";              break;
        case RequestSpeak:       url += "requestSpeak.php";        break;
        case UpdateUserStatus:   url += "updateUserStatus.php";    break;
    };

    QStringList parameterString;
    for(int i = 0; i < parameters.count(); ++i) {
        QPair<QString, QString> current = parameters[i];
        parameterString << current.first + "=" + current.second;
    }

    if(!parameterString.isEmpty())
        url += "?" + parameterString.join("&");

    QNetworkRequest req((QUrl(url)));
    if(endpoint != Login)
        req.setRawHeader("Authorization", _currentToken.toLocal8Bit());

    return req;
}

QJsonObject LFClientAPIInterface::handleReply(QNetworkReply *r, bool *ok, int *httpCode) {
    const int statusCode = r->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if(httpCode)
        *httpCode = statusCode;

    if(ok)
        *ok = statusCode == 200;

    QJsonParseError err;
    QJsonObject obj = QJsonDocument::fromJson(r->readAll(), &err).object();
    if(err.error != QJsonParseError::NoError) {
        // error parsing json
        if(ok) *ok = false;
        qCritical() << "failed parsing json: " << err.errorString();
        return QJsonObject();
    } else {
        if(statusCode != 200) {
            const QString errorName = obj.value("error").toString();
            if(errorName == "login_invalid") _errorType = LoginInvalid;
            else if(errorName == "user_not_found") _errorType = LoginUserNotFound;
            else if(errorName == "wrong_password") _errorType = LoginWrongPassword;
            else if(errorName == "unauthorized") _errorType = Unauthorized;
            else if(errorName == "access_denied") _errorType = AccessDenied;
            else if(errorName == "invalid_url_parameters") _errorType = InvalidParams;
            else if(errorName == "sql_request_error") _errorType = SqlRequestError;
            else if(errorName == "user_not_found") _errorType = UserNotFound;
            else if(errorName == "speak_request_already_exists") _errorType = SpeakRequestAlreadyExists;
            else if(errorName == "speak_request_does_not_exists") _errorType = SpeakRequestDoesNotExist;
            _errorString = obj.value("error_description").toString();
        }
    }

    return obj;
}
