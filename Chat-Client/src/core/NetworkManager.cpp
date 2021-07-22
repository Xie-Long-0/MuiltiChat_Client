#include "NetworkManager.h"
#include <QJsonObject>
#include <QJsonDocument>

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent),
    tcpSocket(new QTcpSocket(this))
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
}

void NetworkManager::login(const QString &username, const QString &encryptedPassword)
{
    QJsonObject json;
    json["type"] = "login";
    json["username"] = username;
    json["password"] = encryptedPassword;

    tcpSocket->connectToHost("127.0.0.1", 12345);
    if (tcpSocket->waitForConnected())
    {
        tcpSocket->write(QJsonDocument(json).toJson());
    }
    else
    {
        emit loginFailed("Connection timeout");
    }
}

void NetworkManager::onReadyRead()
{
    QByteArray data = tcpSocket->readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(data);
    QJsonObject response = responseDoc.object();

    qDebug() << response;

    if (response["status"] == "success")
    {
        emit loginSuccessful();
    }
    else
    {
        emit loginFailed("Invalid username or password");
    }
}
