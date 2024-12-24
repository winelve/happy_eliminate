#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QDebug>

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);

    static QString currentUsername;
    static int currentUserkey;

    //1.设置当前登录的用户名
    Q_INVOKABLE static void setUsername(const QString &username) {
        currentUsername = username;
        qDebug() << "玩家名：" << currentUsername;
    }

    //2.获取当前用户名
    Q_INVOKABLE static QString getUsername() {
        return currentUsername;
    }

    //3.设置当前登录的用户密码
    Q_INVOKABLE static void setUserkey(const int &userkey) {
        currentUserkey = userkey;
        qDebug() << "玩家密码：" << currentUserkey;
    }

    //4.获取当前用户密码
    Q_INVOKABLE static int getUserkey() {
        return currentUserkey;
    }

};
#endif // USERMANAGER_H
