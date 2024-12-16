#ifndef BOARD_H
#define BOARD_H

#include "cube.h"
#include "vector2.h"
#include "Animation/animationfactory.h"
#include "Animation/animationmanager.h"

#include <QObject>
#include <vector>
#include <memory> // 引入智能指针
#include <QDebug>

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(const std::vector<std::vector<int>> &board, QObject *parent = nullptr);
    explicit Board(const std::vector<std::vector<Cube>> &board, QObject *parent = nullptr);
    explicit Board(int width, int height, QObject *parent = nullptr);
    Board(QObject *parent = nullptr);

    const static int ktype_size;

    void PrintBoard();
    void SetCubesToMove(std::vector<std::vector<Vector2>> data) { cubes_to_remove_ = data; }

    std::shared_ptr<Cube> GetCube(int row, int col) {
        if (row < 0 || row >= GetHeight() || col < 0 || col >= GetWidth()) {
            qDebug() << "GetCube: Position out of bounds (" << row << "," << col << ")";
            return nullptr;
        }
        return board_[row][col];
    }

    std::shared_ptr<Cube> GetCube(Vector2 pos) {
        return GetCube(pos.GetRow(), pos.GetColumn());
    }

    void SetCube(Vector2 pos, std::shared_ptr<Cube> cube);
    int GetWidth() const { return board_.empty() ? 0 : board_[0].size(); }
    int GetHeight() const { return board_.size(); }

    void Swap(const Vector2 &pos_1, const Vector2 &pos_2);

    void RunGameLogic(const Vector2 &pos_1, const Vector2 &pos_2);

    static QPointF GetRenderPos(int row, int col);
    static QPointF GetRenderPos(Vector2 pos);


    void HandleMouseClick(const Vector2 &pos);
    void ResetSelection();
    Vector2 first_click_pos_;
    Vector2 second_click_pos_;
    bool choose_one_ = false;
    bool areAdjacent(const Vector2 &pos1, const Vector2 &pos2) const;

private:
    std::vector<std::vector<std::shared_ptr<Cube>>> board_; // 游戏棋盘
    std::vector<std::vector<Vector2>> cubes_to_remove_; // 已分类的,要清理的cube
    AnimationFactory& ani_factory_ = AnimationFactory::GetInstance();
    AnimationManager* ani_manager_ = AnimationManager::GetInstance();

    void InitBoard(const std::vector<std::vector<int>> &board);
    void InitBoard(const std::vector<std::vector<Cube>> &board);
    void InitRandomBoard(int width, int height); // 生成一个随机的游戏初始地图
    bool CausesMatch(int row, int col, int type); // 辅助函数：检查是否形成消除组合

    void DelCube(int row, int col); // 删除一个单元
    void OnCubeDel(); // 用于处理cube删除后的逻辑
    std::shared_ptr<Cube> GenerateCube(); // 生成随机新的方块

    // 游戏规则
    std::vector<std::vector<Vector2>> CheckBoard(); // 检查当前棋盘是否有可消除元素
    int ClearCube(const std::vector<std::vector<Vector2>> &cubes_remove); // 清除给定的方块

public:
    void Fall();
    void OnSwap(); // 处理交换后的逻辑
    void Fill(); // 填充

    // 下面是实验性新增状态机
    void InitializeFSM(); // 初始化状态机

signals:
    // 定义FSM所需的信号
    void userSelectedTwoPieces(const Vector2 &pos1, const Vector2 &pos2);
    void swapAnimationFinished();
    void matchFound();
    void noMatch();
    void clearAnimationFinished();
    void fallAnimationFinished();
    void newMatchFound();
    void noNewMatch();

    // 信号用于通知界面更新
    void boardUpdated();

private:
    // 定义游戏状态
    enum class State {
        WaitingForInput,
        Swapping,
        CheckingMatch,
        Clearing,
        Falling,
        EndCheck
    };

    State currentState_; // 当前状态

    // 状态处理函数
    void enterWaitingForInput();
    void enterSwapping();
    void enterCheckingMatch();
    void enterClearing();
    void enterFalling();
    void enterEndCheck();

    // 状态转换函数
    void transitionTo(State newState);

    // 当前选中的两个位置
    Vector2 first_pos_;
    Vector2 second_pos_;

    // 其他现有的成员变量...

    // 初始化状态机的辅助函数
    void setupStateTransitions();

    // FSM相关
    void setupConnections();

private:


    std::vector<std::vector<Vector2>> find_row_line4(const std::vector<std::vector<Vector2>>& match);
    std::vector<std::vector<Vector2>> find_col_line4(const std::vector<std::vector<Vector2>>& match);
    std::vector<Vector2> falling_cubes_;
    void RemoveFromMatch(const Vector2 &pos,std::vector<std::vector<Vector2>> &mathces);
    void SetRow4(std::vector<std::vector<Vector2>> &mathces);
    void SetCol4(std::vector<std::vector<Vector2>> &mathces);
    std::vector<std::vector<Vector2>> ProcessSpecialMatches(const std::vector<std::vector<Vector2>> &matches);
};

#endif // BOARD_H
