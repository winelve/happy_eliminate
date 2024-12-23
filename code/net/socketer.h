#ifndef SOCKETER_H
#define SOCKETER_H

#include <QObject>
#include <QTcpSocket>

class Socketer : public QObject
{
    Q_OBJECT
public:
    explicit Socketer(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    void sendMessage(const QString &message);
    QList<QPair<QString, int>> getUserScores();
    void addNewScores(QString name,int score);
    QTcpSocket *getTcpSocket();
signals:
    void messageReceived(const QString &message);
    void ranksReceived(const QList<QPair<QString, int>> &scores);
private slots:
    void onReadyRead();
    void onConnected();
    void onError(QAbstractSocket::SocketError socketError);


private:
    QTcpSocket *tcpSocket;
    QList<QPair<QString, int>> userScores;
};

#endif // SOCKETER_H
