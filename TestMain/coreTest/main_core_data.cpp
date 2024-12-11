
#include <QApplication>

//test
#include "code/core/board.h"
#include <vector>
using namespace std;
void test(){
    vector<vector<int>> matrix = {
        {1, 2, 1, 4, 5},
        {1, 1, 1, 1, 10},
        {11, 12, 1, 14, 15},
        {16, 17, 18, 19, 20},
        {2, 2, 2, 2, 2},
        {5, 4, 2, 3, 2},
        {4, 3, 1, 1, 2}
    };
    srand(static_cast<unsigned int>(time(nullptr))); // 初始化随机数生成器
    // Board board(8,8);
    Board board(matrix);
    board.test_check_board();
}
//


int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);
    qDebug() << "Hello happy_elimitate" ;
    test();
    // return a.exec();
    return 0;
}


//  属于 Board 的测试函数
    void test_check_board(){
        PrintBoard();
        qDebug() << "";
        std::vector<std::vector<Vector2>> pos_list = CheckBoard();
        qDebug() << "size:" << pos_list.size();
        ClearCube(pos_list);
        qDebug() << "\nAfter:";
        PrintBoard();
        Swap(Vector2(0,2),Vector2(0,3));
        qDebug() <<  "\nSwap:";
        PrintBoard();
        // Fall();
        // qDebug() << "\nFall:";
        // PrintBoard();
        // Fill();
        // qDebug() << "\nFill:";
        // PrintBoard();
        // qDebug() << "\nNewDel:";
        // ClearCube(CheckBoard());
        // PrintBoard();
    }