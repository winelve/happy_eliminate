#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include "code/database/database.h"


class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr); // 带参数的构造函数

private slots:
    void onNewConnection();
    void onReadyRead();

private:

    DataBase db; // 添加 DataBase 成员变量
    void startServer(); // 私有函数，用于启动服务器

};

#endif // SERVER_H
