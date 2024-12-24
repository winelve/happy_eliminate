#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>

class UserManager {
public:
    static QString currentUsername;

    // 设置当前登录的用户名
    static void setUsername(const QString &username) {
        currentUsername = username;
    }

    // 获取当前用户名
    static QString getUsername() {
        return currentUsername;
    }
};

QString UserManager::currentUsername = "";


#endif // USERMANAGER_H
