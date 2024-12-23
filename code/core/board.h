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

    void PrintBoard();
private:
    std::vector<std::vector<std::shared_ptr<Cube>>> board_; // 棋盘数据

    void InitBoard(const std::vector<std::vector<int>> &board);
    void InitBoard(const std::vector<std::vector<Cube>> &board);
    void InitRandomBoard(int width, int height); // 生成一个随机的游戏初始地图
    bool CausesMatch(int row, int col, int type); // 辅助函数：检查是否形成消除组合
    void DelCube(int row, int col); // 删除一个单元



public:

    //<<<<<<<<<<<<<<需要分离 控制层
    void RunGameLogic(const Vector2 &pos_1, const Vector2 &pos_2);//这个东西应该在控制层来实现
    //>>>>>>>>>>>>>>需要分离






    // //纯脑瘫设计>>>>>>>>>>>>>>>>>>>>>>
    // std::vector<std::vector<Vector2>> find_row_line4(const std::vector<std::vector<Vector2>>& match);
    // std::vector<std::vector<Vector2>> find_col_line4(const std::vector<std::vector<Vector2>>& match);
    // std::vector<Vector2> falling_cubes_;
    // void RemoveFromMatch(const Vector2 &pos,std::vector<std::vector<Vector2>> &mathces);
    // void SetRow4(std::vector<std::vector<Vector2>> &mathces);
    // void SetCol4(std::vector<std::vector<Vector2>> &mathces);
    // std::vector<std::vector<Vector2>> ProcessSpecialMatches(const std::vector<std::vector<Vector2>> &matches);
    // //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
};

#endif // BOARD_H
