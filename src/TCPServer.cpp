#include "TCPServer.h"

// реализуем код серверной части
// сервер может принимать входящие JSON-сообщения от клиентов, обрабатывать их и пересылать всем подключенным клиентам
TCPServer::TCPServer(quint32 TCPPort, QObject *parent) : QObject(parent),
                                                         server(new QTcpServer(this)) {
    initConnections();
    if (server->listen(QHostAddress::Any, TCPPort)) {
        qDebug() << "Server started";
    } else {
        qDebug() << "Server not started";
    }
}

// слот будет вызываться когда устанавливается новое соединение с клиентом
// принимает соединение, сохраняет его в список sockets и устанавливает соединения для чтения данных и обработки отключения
void TCPServer::slotNewConnection() {
    auto socket = server->nextPendingConnection();
    sockets.push_back(socket);
    qDebug() << "[New connection] port =" << socket->localPort() << " host =" << socket->localAddress();

    connect(sockets.last(), &QTcpSocket::readyRead, this, &TCPServer::slotServerRead);
    connect(sockets.last(), &QTcpSocket::disconnected, this, &TCPServer::slotClientDisconnected);
}

// этот слот вызывается, когда клиент отправляет данные на сервер
// читает данные, ожидая получения JSON-сообщений от клиента
// каждое сообщение парсится и передается в функцию jsonReceived() для обработки
void TCPServer::slotServerRead() {
    for (const auto &socket: sockets) {
        QByteArray jsonData;
        QDataStream socketStream(socket);
        socketStream.setVersion(QDataStream::Qt_6_2); // используем QDataStream для разбора JSON-сообщений

        for (;;) {
            socketStream.startTransaction();
            socketStream >> jsonData;
            if (socketStream.commitTransaction()) {
                QJsonParseError parseError;
                const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
                if (parseError.error == QJsonParseError::NoError) {
                    if (jsonDoc.isObject())
                        jsonReceived(jsonDoc.object());
                }
            } else {
                break;
            }
        }
    }


//    while (socket->bytesAvailable() > 0) {
//        auto array = socket->readAll();
//        auto split_data = array.split(';');
//        qDebug() << "Server " << socket->localPort() << " sender: " << split_data[0] << " receiver: " << split_data[1]
//                 << " message: '" << split_data[2] << "'";
//        socket->write(array);
//    }
}

// функция обрабатывает JSON-сообщения от клиента
// проверяет, является ли сообщение типом "message" и извлекает текст сообщения и идентификаторы отправителя и получателя
// затем она генерирует сигнал signalDisplayMessage, чтобы сообщить о полученном сообщении, и отправляет сообщение всем клиентам, подключенным к серверу
void TCPServer::jsonReceived(const QJsonObject &docObj) {
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));

    if (typeVal.isNull() || !typeVal.isString())
        return;

    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0) {

        const QJsonValue textVal = docObj.value(QLatin1String("text"));


        if (textVal.isNull() || !textVal.isString())
            return;

        emit signalDisplayMessage(docObj.value("sender_id").toString().toUtf8(),
                                  docObj.value("receiver_id").toString().toUtf8(), textVal.toString().toUtf8());

        qDebug() << "Received from client:" << textVal << " " << docObj.value("sender_id") << " "
                 << docObj.value("receiver_id");

        for (const auto &socket: sockets) {
            QDataStream clientStream(socket);
            clientStream.setVersion(QDataStream::Qt_6_2);
            clientStream << docObj;
        }
    }
}

// слот вызывается, когда клиент отключается от сервера
// ищет отключившийся сокет в списке sockets и удаляет его
// затем он закрывает соединение и выводит сообщение об отключении
void TCPServer::slotClientDisconnected() {
    auto socket = dynamic_cast<QTcpSocket*>(sender());
    auto iterator_on_sockets = std::find(sockets.begin(), sockets.end(), socket);
    if (iterator_on_sockets != sockets.end()) {
        sockets.erase(iterator_on_sockets);
    }
    socket->close();
    qDebug() << "Server: socket " << socket->localPort() << " disconnected";
}

// функция устанавливает соединение между объектом сервера и слотом slotNewConnection()
// вызывается из конструктора
void TCPServer::initConnections() {
    connect(server, &QTcpServer::newConnection, this, &TCPServer::slotNewConnection);
}
