#ifndef BOARD_H
#define BOARD_H

#include "./entity/cube.h"
#include "./utils/vector2.h"

#include <QObject>
#include <vector>
#include <memory> // 引入智能指针
#include <QDebug>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(const std::vector<std::vector<int>> &board, QWidget *parent = nullptr);
    explicit Board(const std::vector<std::vector<Cube>> &board, QWidget *parent = nullptr);
    explicit Board(int width, int height, QWidget *parent = nullptr);
    Board(QWidget *parent = nullptr);

    std::shared_ptr<Cube> GetCube(int row, int col);
    std::shared_ptr<Cube> GetCube(Vector2 pos);
    void SetCube(Vector2 pos, std::shared_ptr<Cube> cube);
    void SetCube(int row,int col,std::shared_ptr<Cube> cube);

    int GetWidth() const { return board_.empty() ? 0 : board_[0].size(); }
    int GetHeight() const { return board_.size(); }

    bool CausesMatch(int row, int col, int type); // 辅助函数：检查是否形成消除组合
    void PrintBoard();
    void InitRandomBoard(int width, int height); // 生成一个随机的游戏初始地图
private:
    std::vector<std::vector<std::shared_ptr<Cube>>> board_; // 棋盘数据

    void InitBoard(const std::vector<std::vector<int>> &board);
    void InitBoard(const std::vector<std::vector<Cube>> &board);

    void DelCube(int row, int col); // 删除一个单元

};

#endif // BOARD_H
