#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include "./utils/vector2.h"
#include "board.h"

class GameLogic : public QObject
{
    Q_OBJECT

public:
    // 获取单例实例
    static GameLogic& instance();

    // 禁止拷贝和赋值
    GameLogic(const GameLogic&) = delete;
    GameLogic& operator=(const GameLogic&) = delete;


    // 检查棋盘是否有匹配
    std::vector<std::vector<Vector2>> CheckBoard(std::shared_ptr<Board> board);

    // 清除给定的方块
    int ClearCube(std::shared_ptr<Board> board, const std::vector<std::vector<Vector2>>& cubes_remove);

    // 交换棋盘上的两个方块
    void Swap(std::shared_ptr<Board> board, const Vector2& pos_1, const Vector2& pos_2);

    // 处理下落的方块
    void Fall(std::shared_ptr<Board> board);

    // 填充空位
    void Fill(std::shared_ptr<Board> board);

signals:
    void fallEvent(int fromRow, int fromCol, int toRow, int toCol,std::shared_ptr<Cube> cube); // 信号传递下落事件

private:
    explicit GameLogic(QObject *parent = nullptr); // 私有构造函数
};

#endif // GAMELOGIC_H
