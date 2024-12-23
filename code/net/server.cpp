#include "code/net/server.h"

#include <QHostAddress>
#include <QTcpSocket>
#include <QDebug>



Server::Server(QObject *parent) : QTcpServer(parent),db("D:/apps/Qt.file/git/happy_eliminate/db/users.db") {

    db.BuildDatabase();
    startServer();
}

void Server::startServer() {
    listen(QHostAddress::Any, 1234); // 监听1234端口
    connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);

}

void Server::onNewConnection() {
    QTcpSocket *clientSocket = nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

}

void Server::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (clientSocket) {
        QString message = clientSocket->readAll();

        //qDebug() << "Server received:" << message;
        // 在这里处理消息
        QStringList orderParts = message.split(':');
        if(orderParts.isEmpty())
        {
            clientSocket->write("Error: Empty message");
        }else {
            QString orderTotal;
            for(int i=0;i<orderParts.size()-1;i++){

                qDebug()<<"服务器接收到一条指令:"+orderParts[i];

                QStringList parts = orderParts[i].split('_'); // 根据_分割消息


                // 假设我们根据分割后的第一个元素来判断消息类型
                if (parts.isEmpty()) {
                    clientSocket->write("Error: Empty message");
                }
                else if(parts[0]=="111"){

                    QString command = parts[1]; // 获取命令部分

                    if (command == "getRanks") {
                        //处理请求

                        QList<QPair<QString, int>> userScores = db.fetchUsersByScore();

                        for (const auto &userScore : userScores) {
                            QString SendMessage="222_" + userScore.first + "_" + QString::number(userScore.second)+"_";
                            clientSocket->write(SendMessage.toUtf8());

                            //qDebug() << "用户姓名:" << userScore.first << ", 得分:" << userScore.second;
                        }

                    }else if(command=="getRanks successfully"){

                    }

                }else if(parts[0]=="333"){
                    db.updateUserScore(parts[1],parts[2].toInt());
                }



            }

        }



    }
}
