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
const int Magic_Eliminate = 33;
}


class Cube: public Entity
{

private:
    int type_;
    Vector2 pos_; //之所以维护这个是因为迫不得已.-->为了渲染动画的

    int eliminate_status_;


    void InitAniComponent(); //初始化动画组件
public:
        // 默认构造函数，接受父对象
    explicit Cube(QWidget *parent = nullptr);

    // 接受类型和父对象的构造函数
    explicit Cube(int type, QWidget *parent = nullptr);

    // 接受类型、位置和父对象的构造函数
    explicit Cube(int type, const Vector2& pos, QWidget *parent = nullptr);

    void SetType(int type) { type_ = type; }
    int GetType() { return type_; }

    Vector2 GetPos() { return pos_; }
    // void SetPos(const Vector2 &new_pos) { pos_ = new_pos; SetRenderPos(GetRenderPos(pos_));  }
    void SetPos(const Vector2 &new_pos) { pos_ = new_pos;   }

    void SetEliminate(int state);
    int GetEliminate() { return eliminate_status_; }

    bool Empty() { return type_==0 ? true:false; }

    // 获取对应的位置的渲染坐标
    QPointF GetRenderPos(int row, int col) {
        qreal cell_size = static_cast<qreal>(Constants::k_cell_size);
        qreal padding = static_cast<qreal>(Constants::k_board_padding);

        qreal x = padding + col * cell_size;
        qreal y = padding + row * cell_size;

        return QPointF(x, y);
    }

    QPointF GetRenderPos(Vector2 pos) {
        return GetRenderPos(pos.GetRow(), pos.GetColumn());
    }

    //>>>>>>>>>>>>>>>>没有用的东西>>>>>>>>>>>>>>>>>>>
    // bool IsChoosed() { return is_choosed_; }
    // void SetChoosed(bool status) { is_choosed_ = status; }
    // void SetPlaying(bool play) { is_playing_animation_ = play; }
    // int GetSpecialType() { return special_type_; }
    // void SetSpecialType(int type) { special_type_ = type; }
    // void paint(QPainter &painter); //其实也不需要
    // bool is_choosed_;
    // bool is_playing_animation_;
    // QPixmap cube_texture;
    // int special_type_ = 0;
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    bool operator==(const Cube& other) const {
        return type_ == other.type_;
    }
};

#endif // CUBE_H
