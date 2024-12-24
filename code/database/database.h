#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QObject>

class DataBase : public QObject {
    Q_OBJECT
public:
    explicit DataBase(const QString &dbPath = "users.db", QObject *parent = nullptr); // Add this lines
    void BuildDatabase();
    Q_INVOKABLE void updateUserScore(const QString &name, int score,int key); // 新增函数声明
    QList<QPair<QString, int>> fetchUsersByScore(); // 新增函数声明，返回用户的姓名和得分
    Q_INVOKABLE int findPasswordByUserName(const QString &name);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
