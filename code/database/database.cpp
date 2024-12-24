#include "database.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMap>

DataBase::DataBase(const QString &dbPath, QObject *parent) : QObject(parent) {
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
                score INTEGER NOT NULL,
                key INTEGER NOT NULL
            );
        )";

        if (!query.exec(createTableQuery)) {
            qCritical() << "创建表失败:" << query.lastError().text();
        } else {
            qDebug() << "表 'users' 创建成功。";
        }
    } else {
        // 表存在，检查key列是否存在
        QString columnCheckQuery = "PRAGMA table_info('users')";
        if (!query.exec(columnCheckQuery)) {
            qCritical() << "检查列存在失败:" << query.lastError().text();
            db.close();
            return;
        }

        bool keyColumnExists = false;
        while (query.next()) {
            int columnIndex = query.value(0).toInt(); // columnid
            QString columnName = query.value(1).toString(); // name
            int dataType = query.value(2).toInt(); // type
            int dataTypeSize = query.value(3).toInt(); // not used
            bool notNull = query.value(4).toInt() != 0; // notnull
            QString defaultValue = query.value(5).toString(); // dflt_value
            bool primaryKey = query.value(6).toInt() != 0; // pk

            if (columnName == "key") {
                keyColumnExists = true;
                break;
            }
        }

        if (!keyColumnExists) {
            // 如果表已存在，但key列不存在，则添加key列
            QString alterTableQuery = "ALTER TABLE users ADD COLUMN key INTEGER NOT NULL DEFAULT 0";
            if (!query.exec(alterTableQuery)) {
                qCritical() << "添加列失败:" << query.lastError().text();
            } else {
                qDebug() << "列 'key' 添加成功。";
            }
        }
    }

    db.close(); // 操作完成后确保关闭数据库
}

void DataBase::updateUserScore(const QString &name, int score, int key) {
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
        query.prepare("UPDATE users SET score = :newScore, key = :key WHERE name = :name");
        query.bindValue(":name", name);
        query.bindValue(":newScore", newScore);
        query.bindValue(":key", key);

        if (!query.exec()) {
            qCritical() << "更新失败:" << query.lastError().text();
        } else {
            qDebug() << "用户 " << name << " 的得分更新成功，新得分为" << newScore;
        }
    } else {
        query.prepare("INSERT INTO users (name, score, key) VALUES (:name, :score, :key)");
        query.bindValue(":name", name);
        query.bindValue(":score", score);
        query.bindValue(":key", key);
        if (!query.exec()) {
            qCritical() << "插入失败:" << query.lastError().text();
        } else {
            qDebug() << "新用户 " << name << " 添加成功，得分为" << score;
        }
    }

    db.close();
}
QList<QPair<QString, int>> DataBase::fetchUsersByScore() {
    QList<QPair<QString, int>> userScores;

    if (!db.open()) {
        qCritical() << "数据库打开失败:" << db.lastError().text();
        return userScores; // 返回空list
    }

    QSqlQuery query(db);
    query.exec("SELECT name, score FROM users ORDER BY score DESC");
    while (query.next()) {
        QString name = query.value(0).toString();
        int score = query.value(1).toInt();
        userScores.append(qMakePair(name, score));
    }

    db.close(); // 操作完成后关闭数据库连接
    return userScores; // 返回包含用户姓名和得分的list
}
int DataBase::findPasswordByUserName(const QString &name) {
    int key = 0; // 假设key是int类型，并初始化为0
    if (!db.open()) {
        qCritical() << "数据库打开失败:" << db.lastError().text();
        return key; // 返回0
    }

    QSqlQuery query(db);
    query.prepare("SELECT key FROM users WHERE name = :name"); // 准备查询语句以选择key
    query.bindValue(":name", name); // 绑定查询参数
    if (!query.exec()) {
        qCritical() << "查询执行失败:" << query.lastError().text();
        db.close();
        return key; // 返回0
    }

    if (query.next()) {
        key = query.value(0).toInt(); // 获取key值，它是查询结果的第一列
    }

    db.close(); // 操作完成后关闭数据库连接
    return key; // 返回找到的key或0
}

