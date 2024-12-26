#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <memory>

class Board;

class BoardManager : public QObject
{
    Q_OBJECT

public:
    // 获取单例实例
    static BoardManager& instance();

    // 禁止拷贝和赋值
    BoardManager(const BoardManager&) = delete;
    BoardManager& operator=(const BoardManager&) = delete;

    // 检查是否已初始化（是否有 Board 数据）
    bool IsInitialized() const;

    // 添加新的 Board
    void AddBoard(const QString& name, std::shared_ptr<Board> board);

    // 切换到指定的 Board
    bool SwitchToBoard(const QString& name);

    // 获取当前活动的 Board
    std::shared_ptr<Board> GetCurrentBoard() const;
    void ResetManager();

private:
    explicit BoardManager(QObject *parent = nullptr); // 私有构造函数

    QMap<QString, std::shared_ptr<Board>> boardPool_; // Board 池
    std::shared_ptr<Board> currentBoard_;            // 当前活动的 Board

};

#endif // BOARDMANAGER_H
