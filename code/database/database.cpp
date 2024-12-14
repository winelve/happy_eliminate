#include "database.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMap>

DataBase::DataBase(const QString &dbPath) {
    // Initialize database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath.isEmpty() ? "users.db" : dbPath);
}

void DataBase::BuildDatabase() {
    // 尝试打开数据库
    if (!db.open()) {
        qCritical() << "数据库打开失败:" << db.lastError().text();
        return;
    }

    QSqlQuery query(db);

    // 检查表是否存在
    if (!query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='users'")) {
        qCritical() << "检查表存在失败:" << query.lastError().text();
        db.close();
        return;
    }

    if (!query.next()) {
        // 表不存在，创建表
        const QString createTableQuery = R"(
            CREATE TABLE users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                score INTEGER NOT NULL
            );
        )";

        if (!query.exec(createTableQuery)) {
            qCritical() << "创建表失败:" << query.lastError().text();
        } else {
            qDebug() << "表 'users' 创建成功。";
        }
    } else {
        qDebug() << "表 'users' 已存在。";
    }

    db.close(); // 操作完成后确保关闭数据库
}

void DataBase::updateUserScore(const QString &name, int score) {
    if (!db.open()) {
        qCritical() << "数据库打开失败:" << db.lastError().text();
        return;
    }

    QSqlQuery query(db);

    query.prepare("SELECT score FROM users WHERE name = :name");
    query.bindValue(":name", name);
    if (!query.exec()) {
        qCritical() << "查询执行失败:" << query.lastError().text();
        db.close();
        return;
    }

    if (query.next()) {
        int currentScore = query.value(0).toInt();
        int newScore = currentScore + score;
        query.prepare("UPDATE users SET score = :newScore WHERE name = :name");
        query.bindValue(":newScore", newScore);
        query.bindValue(":name", name);
        if (!query.exec()) {
            qCritical() << "更新失败:" << query.lastError().text();
        } else {
            qDebug() << "用户 " << name << " 的得分更新成功，新得分为" << newScore;
        }
    } else {
        query.prepare("INSERT INTO users (name, score) VALUES (:name, :score)");
        query.bindValue(":name", name);
        query.bindValue(":score", score);
        if (!query.exec()) {
            qCritical() << "插入失败:" << query.lastError().text();
        } else {
            qDebug() << "新用户 " << name << " 添加成功，得分为" << score;
        }
    }

    db.close();
}

QMap<QString, int> DataBase::fetchUsersByScore() {
    QMap<QString, int> userScores;

    if (!db.open()) {
        qCritical() << "数据库打开失败:" << db.lastError().text();
        return userScores; // 返回空map
    }

    QSqlQuery query(db);
    query.exec("SELECT name, score FROM users ORDER BY score DESC");
    while (query.next()) {
        QString name = query.value(0).toString();
        int score = query.value(1).toInt();
        userScores.insert(name, score);
    }

    db.close(); // 操作完成后关闭数据库连接
    return userScores; // 返回包含用户姓名和得分的map
}



