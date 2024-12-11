#ifndef BOARD_H
#define BOARD_H

#include "cube.h"
#include "vector2.h"

#include <QObject>
#include<vector>
#include <QDebug>

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(const std::vector<std::vector<int>> &board,QObject *parent = nullptr);
    explicit Board(const std::vector<std::vector<Cube>> &board,QObject *parent = nullptr);
    explicit Board(int width,int height,QObject *parent = nullptr);

    const static int ktype_size;

    void PrintBoard();
    void SetCubesToMove(std::vector<std::vector<Vector2>> data) { cubes_to_remove_ = data; }

    Cube GetCube(int row,int col) const { return board_[row][col]; };
    Cube GetCube(Vector2 pos) const { return board_[pos.GetRow()][pos.GetColumn()]; }

    void SetCube(Vector2 pos,const Cube cube);
    int GetWidth() { return board_.size()>0 ? board_[0].size() : 0; }
    int GetHeight() { return board_.size(); }

    void test_check_board(){
        PrintBoard();
        qDebug() << "";
        std::vector<std::vector<Vector2>> pos_list = CheckBoard();
        qDebug() << "size:" << pos_list.size();
        ClearCube(pos_list);
        qDebug() << "\nAfter:";
        PrintBoard();
        Fall();
        qDebug() << "\nFall:";
        PrintBoard();
    }


private:
    std::vector<std::vector<Cube>> board_; //游戏棋盘
    std::vector<std::vector<Vector2>> cubes_to_remove_; //已分类的,要清理的cube

    void InitBoard(const std::vector<std::vector<int>> &board);
    void InitBoard(const std::vector<std::vector<Cube>> &board);
    void InitRandomBoard(int width,int height); //生成一个随机的游戏初始地图
    bool CausesMatch(int row, int col, int type); // 辅助函数：检查是否形成消除组合

    void DelCube(int row,int col); //删除一个单元
    void OnCubeDel(); //用于处理cube删除后的逻辑

    //游戏规则
    std::vector<std::vector<Vector2>> CheckBoard(); //检查当前棋盘是否有可消除元素
    int ClearCube(std::vector<std::vector<Vector2>> cubes_remove); // << 清楚给定的方块
    void Fall();
    void Swap(Vector2 pos_1,Vector2 pos_2);
    void OnSwap(); //处理交换后的逻辑

};

#endif // BOARD_H
