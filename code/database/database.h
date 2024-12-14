#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>

class DataBase {
public:
    explicit DataBase(const QString &dbPath = "users.db"); // Add this line
    void BuildDatabase();
    void updateUserScore(const QString &name, int score); // 新增函数声明
    QMap<QString, int> fetchUsersByScore(); // 新增函数声明，返回用户的姓名和得分

private:
    QSqlDatabase db;
};

#endif // DATABASE_H

