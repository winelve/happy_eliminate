#ifndef VECTOR2_H
#define VECTOR2_H
#include <QPixmap>

class Vector2
{

private:
    int row_;
    int column_;

public:
    Vector2();
    Vector2(int row,int colum);

    void SetRow(int row);
    int GetRow() const;
    void SetColumn(int col);
    int GetColumn() const;
    void MoveTo(int row,int column);  //移动到指定的向量处
    void MoveTo(Vector2 other);
    void Add(int row,int col);
    void Add(Vector2 other);

};

#endif // VECTOR2_H
