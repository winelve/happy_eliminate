#include "utils.h"
#include <random>
#include <unordered_set>
#include "code/audio/audioplayer.h"

namespace Utils {

// 辅助函数：检查两个位置是否相邻
bool areAdjacent(const Vector2 &pos1, const Vector2 &pos2) {
    int rowDiff = abs(pos1.GetRow() - pos2.GetRow());
    int colDiff = abs(pos1.GetColumn() - pos2.GetColumn());
    // 检查是否在上下左右相邻
    return ((rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1));
}

// 生成一个新的随机方块
std::shared_ptr<Cube> GenerateCube() {
    // 使用随机设备初始化随机数生成器
    static std::random_device rd;    // 随机设备，提供种子
    static std::mt19937 gen(rd());  // 使用 Mersenne Twister 算法的随机数引擎
    static std::uniform_int_distribution<> dist(1, Constants::ktype_size); // 定义均匀分布范围

    int new_type = dist(gen); // 生成 [1, ktype_size] 范围内的随机数
    qDebug() << "new_type: " << new_type;
    return std::make_shared<Cube>(new_type);
}

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

// Function to output the matches array
void PrintMatches(const std::vector<std::vector<Vector2>> &matches) {
    for (size_t i = 0; i < matches.size(); ++i) {
        qDebug() << "Match" << i + 1 << ":";
        for (const auto &vec : matches[i]) {
            qDebug() << "(" << vec.GetRow() << "," << vec.GetColumn() << ")";
        }
    }
}

void RemoveDuplicates(std::vector<std::vector<Vector2>> &matches) {
    for (auto &innerVec : matches) {
        // Use an unordered_set to track unique Vector2 elements
        std::unordered_set<Vector2> uniqueElements;
        auto it = innerVec.begin();

        while (it != innerVec.end()) {
            // If the element is already in the set, erase it
            if (!uniqueElements.insert(*it).second) {
                it = innerVec.erase(it);
                qDebug() << "!!!!!!!!!!!!!!!!!!!!!!重复!!!!!!!!!!!!!!!!!!!";
            } else {
                ++it;
            }
        }
    }
}

void PlayLevelMusic(int times) {
    AudioPlayer* audio = AudioPlayer::getInstance();
    if(times == 2) audio->PlaySoundEffect("contnuousMatch3.mp3");
    else if(times == 3) audio->PlaySoundEffect("contnuousMatch5.mp3");
    else if(times == 4) audio->PlaySoundEffect("contnuousMatch7.mp3");
    else if(times == 5) audio->PlaySoundEffect("contnuousMatch9.mp3");
    else if(times == 6) audio->PlaySoundEffect("contnuousMatch11.mp3");
}

void PlayEliminateMusic(int times){
    AudioPlayer* audio = AudioPlayer::getInstance();
    if(times == 1) audio->PlaySoundEffect("eliminate1.mp3");
    else if(times == 2) audio->PlaySoundEffect("eliminate2.mp3");
    else if(times == 3) audio->PlaySoundEffect("eliminate3.mp3");
    else if(times == 4) audio->PlaySoundEffect("eliminate4.mp3");
    else if(times == 5) audio->PlaySoundEffect("eliminate5.mp3");
    else audio->PlaySoundEffect("eliminate6.mp3");
}



} // namespace Utils
