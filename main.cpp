
#include <QApplication>

//test
#include "code/core/board.h"
#include "code/windows/gamewidget.h"
#include "code/core/Animation/resourcemanager.h"
#include <vector>

using namespace std;
void test(){
    // vector<vector<int>> matrix = {
    //                               {1, 1, 2, 1, 5},
    //                               {3, 6, 1, 2, 6},
    //                               {1, 2, 1, 3, 5},
    //                               };
    // srand(static_cast<unsigned int>(time(nullptr))); // 初始化随机数生成器
    // // Board board(8,8);
    // Board board(matrix);
    // board.test_check_board();

}
//


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "Hello happy_elimitate" ;
    test();
    GameWidget *widget = new GameWidget();
    widget->show();
    ResourceManager::Instance();

    return a.exec();
    // return 0;
}
