#ifndef CUBE_H
#define CUBE_H

#include <QObject>
#include <QDebug>
#include "vector2.h"

class Cube
{

private:
    int type_;
    Vector2 pos_; //之所以维护这个是因为迫不得已.-->为了渲染动画的

    bool is_choosed_;
    bool is_playing_animation_;
    QPixmap cube_texture;

public:
    Cube();
    Cube(int type);

    void SetType(int type) { type_ = type; }
    int GetType() { return type_; }
    bool Empty() { return type_==0 ? true:false; }
    Vector2 GetPos() { return pos_; }
    void SetPos(const Vector2 &new_pos) { pos_ = new_pos; }
    bool IsChoosed() { return is_choosed_; }
    void SetChoosed(bool status) { is_choosed_ = status; }
    void SetPlaying(bool play) { is_playing_animation_ = play; }

    void paint(QPainter &painter);

    bool operator==(const Cube& other) const {
        return type_ == other.type_;
    }
};

#endif // CUBE_H
