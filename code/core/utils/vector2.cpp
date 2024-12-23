#include "vector2.h"


Vector2::Vector2() : row_(0), column_(0) {}

Vector2::Vector2(int row, int column) : row_(row), column_(column) {}

void Vector2::SetRow(int row) {
    row_ = row;
}

int Vector2::GetRow() const {
    return row_;
}

void Vector2::SetColumn(int column) {
    column_ = column;
}

int Vector2::GetColumn() const {
    return column_;
}

// 移动到指定的行和列
void Vector2::MoveTo(int row, int column) {
    row_ = row;
    column_ = column;
}

// 移动到另一个 Vector2 的位置
void Vector2::MoveTo(Vector2 other) {
    row_ = other.GetRow();
    column_ = other.GetColumn();
}

// 增加行和列的值
void Vector2::Add(int row, int column) {
    row_ += row;
    column_ += column;
}

// 增加另一个 Vector2 的行和列的值
void Vector2::Add(Vector2 other) {
    row_ += other.GetRow();
    column_ += other.GetColumn();
}
