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

    void InitBoard(const std::vector<std::vector<int>> &board);
    void InitBoard(const std::vector<std::vector<Cube>> &board);
    void PrintBoard();
    void SetCubesToMove(std::vector<std::vector<Vector2>> data) { cubes_to_remove_ = data; }

    Cube GetCube(int row,int col) const { return board_[row][col]; };
    void SetBoard(int row,int col,int type);


    void test_check_board(){
        PrintBoard();
        qDebug() << "";
        std::vector<std::vector<Vector2>> pos_list = CheckBoard();
        qDebug() << "size:" << pos_list.size();
        // for(auto &list: pos_list){
        //     qDebug() << "Group:" ;
        //     for(Vector2 &pos:list){
        //         qDebug() << "(" << pos.GetRow() << "," << pos.GetColumn() << ")";
        //     }
        //     qDebug() << "----------";
        // }
        ClearCube(pos_list);
        qDebug() << "After:";
        PrintBoard();
    }


private:
    std::vector<std::vector<Cube>> board_; //游戏棋盘
    std::vector<std::vector<Vector2>> cubes_to_remove_; //已分类的,要清理的cube

    //游戏规则
    std::vector<std::vector<Vector2>> CheckBoard(); //检查当前棋盘是否有可消除元素
    int ClearCube(std::vector<std::vector<Vector2>> cubes_remove); // << 清楚给定的方块
    void Fall();


};

#endif // BOARD_H
