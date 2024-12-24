#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>

class DataBase {
public:
    explicit DataBase(const QString &dbPath = "users.db"); // Add this line
    void BuildDatabase();
    void updateUserScore(const QString &name, int score,int key); // 新增函数声明
    QList<QPair<QString, int>> fetchUsersByScore(); // 新增函数声明，返回用户的姓名和得分
    int findPasswordByUserName(const QString &name);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
