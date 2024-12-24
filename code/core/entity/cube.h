#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include <QDebug>
#include "../utils/vector2.h"
#include "../utils/constants.h"
#include "entity.h"

namespace CubeState {
const int Normal_Eliminate = 0;
const int Col_Eliminate = 1;
const int Row_Eliminate = 2;
const int Magic_Eliminate = 3;
}


class Cube: public Entity
{

private:
    int type_;
    Vector2 pos_; //之所以维护这个是因为迫不得已.-->为了渲染动画的

    int eliminate_status_ = CubeState::Normal_Eliminate;


    void InitAniComponent(); //初始化动画组件
public:

    explicit Cube(QWidget *parent = nullptr);
    explicit Cube(int type, QWidget *parent = nullptr);
    explicit Cube(int type, const Vector2& pos, QWidget *parent = nullptr);

    void SetType(int type) { type_ = type; }
    int GetType() { return type_; }

    Vector2 GetPos() { return pos_; }
    void SetPos(const Vector2 &new_pos) { pos_ = new_pos; }

    void SetEliminate(int state) {
        if(state==1) SetState("col");
        else if(state==2) SetState("row");
        else if(state==3) SetState("magic");
        eliminate_status_ = state;
    }
    int GetEliminate() { return eliminate_status_; }

    bool Empty() { return type_==0 ? true:false; }

    // 获取对应的位置的渲染坐标
    QPointF GetRenderPos(int row, int col) {
        qreal cell_size = static_cast<qreal>(Constants::k_cell_size);
        QPoint padding = static_cast<QPoint>(Constants::k_board_padding);
        qreal x = padding.x() + col * cell_size;
        qreal y = padding.y() + row * cell_size;
        return QPointF(x, y);
    }
    QPointF GetRenderPos(Vector2 pos) {
        return GetRenderPos(pos.GetRow(), pos.GetColumn());
    }

    bool operator==(const Cube& other) const {
        return type_ == other.type_;
    }
};

#endif // CUBE_H
