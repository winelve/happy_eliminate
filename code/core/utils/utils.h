#ifndef UTILS_H
#define UTILS_H

#include "vector2.h"
#include "constants.h"
#include "../entity/cube.h"

namespace Utils {

// 辅助函数：检查两个位置是否相邻
bool areAdjacent(const Vector2 &pos1, const Vector2 &pos2);

// 生成一个新的随机方块
std::shared_ptr<Cube> GenerateCube();

// 获取对应的位置的渲染坐标
QPointF GetRenderPos(int row, int col);
QPointF GetRenderPos(Vector2 pos);
void PrintMatches(const std::vector<std::vector<Vector2>> &matches);
}


#endif // UTILS_H
