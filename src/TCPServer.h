#ifndef IMESSAGE_TCPSERVER_H
#define IMESSAGE_TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>

class TCPServer : public QObject {
Q_OBJECT;
public:
    explicit TCPServer(quint32 TCPPort, QObject *parent = nullptr);

signals:

    void signalDisplayMessage(QByteArray, QByteArray, QByteArray);

public slots:

    void slotNewConnection();

    void slotServerRead();

    void slotClientDisconnected();

private:
    QTcpServer *server;
    QVector<QTcpSocket *> sockets;
//    QTcpSocket *socket;

    void initConnections();

    void jsonReceived(const QJsonObject &docObj);
};

#endif //IMESSAGE_TCPSERVER_H
