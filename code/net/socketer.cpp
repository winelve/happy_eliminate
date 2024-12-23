#include "code/net/socketer.h"
#include <QHostAddress>
#include <QDebug>


Socketer::Socketer(QObject *parent) : QObject(parent), tcpSocket(new QTcpSocket(this))
{
    connect(tcpSocket, &QTcpSocket::connected, this, &Socketer::onConnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Socketer::onReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &Socketer::onError);
}

void Socketer::connectToServer(const QString &host, quint16 port)
{
    tcpSocket->connectToHost(host, port);
}

void Socketer::disconnectFromServer()
{
    tcpSocket->disconnectFromHost();
}

void Socketer::sendMessage(const QString &message)
{
    tcpSocket->write(message.toUtf8()+":");
}

void Socketer::onReadyRead()
{
    QString message = tcpSocket->readLine();
    //qDebug() << "Client received:" << message;



    QStringList parts = message.split('_'); // 根据_分割消息

    // 假设我们根据分割后的第一个元素来判断消息类型
    if (parts.isEmpty()) {
        qDebug()<<"消息空的";
    }
    else if(parts[0]=="222"){


        // 从parts列表中提取用户名和分数
        for (int i = 1; i < parts.size(); i += 3) {
            if (i + 1 < parts.size()) { // 确保有分数部分
                QString name = parts[i];
                int score = parts[i + 1].toInt(); // 将分数从字符串转换为整数
                userScores.append(qMakePair(name, score)); // 添加到列表中
            }
        }

        if(!userScores.isEmpty()){
            emit ranksReceived(userScores); // 发出信号
        }

    }


    emit messageReceived(message);
}

void Socketer::onConnected()
{
    qDebug() << "Client connected to the server.";
}

void Socketer::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << tcpSocket->errorString();
}

QList<QPair<QString, int>> Socketer::getUserScores(){

    for (const auto &userScore : userScores) {
        qDebug() << "用户姓名:" << userScore.first << ", 得分:" << userScore.second;
    }

    return userScores;
}

void Socketer::addNewScores(QString name,int score){
    QString message="333_"+name+"_"+QString::number(score);
    sendMessage(message.toUtf8());
}

QTcpSocket * Socketer::getTcpSocket(){
    return tcpSocket;
}
