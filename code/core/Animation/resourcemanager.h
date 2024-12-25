// ResourceManager.h
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include <vector>
#include <QString>
#include <QPixmap>
#include <QRect>
#include <QDebug>


namespace AnimalType {
const int Bear = 1;
const int Cat = 2;
const int Chicken = 3;
const int Fox = 4;
const int Frog = 5;
const int Horse = 6;
};

namespace AnimationType {
const int Click = 1;
const int Col = 2;
const int Row = 3;
const int Wrap = 4;
const int Magic = 5;
};


class ResourceManager
{
public:
    // 获取单例实例
    static ResourceManager& Instance();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    std::vector<QPixmap> GetAniResource(int animal, int animType) const;
    std::vector<QPixmap> GetMagic() const { return magic_texture_list_; }
    std::vector<QPixmap> GetMagicEffect() const { return magic_effect_texture_list_; }
    std::vector<QPixmap> GetStateImg(int state) const { return state==0?game_over_img_:win_img_; }
    QPixmap GetCube(int type) { return cube_texture_list_[type]; }

    std::vector<QPixmap> GetDestroyEffect() { return destroy_texture_list_; }
    QPixmap GetHLine() { return h_line; }
    QPixmap GetVLine() { return v_line; }

    QPixmap GetWordEffect(int times);

private:
    // 私有构造函数
    ResourceManager();
    // 资源存储结构，使用资源ID作为键
    bool ImportResource(int animal, int animType, const QString& path, int num);

    int CalculateResourceID(int animal, int animType) const;
    static std::vector<QPixmap> LoadAtlas(const QString& path, int num);
    QPixmap LoadSingleImg(const QString &path);
    void LoadAllAniResources(); //导入所有的动画资源



    std::unordered_map<int, std::vector<QPixmap>> ani_resources_;
    std::vector<QPixmap> cube_texture_list_;
    std::vector<QPixmap> magic_texture_list_;
    std::vector<QPixmap> destroy_texture_list_;
    std::vector<QPixmap> magic_effect_texture_list_;
    std::vector<QPixmap> word_effect_list_;
    std::vector<QPixmap> win_img_;
    std::vector<QPixmap> game_over_img_;
    QPixmap h_line;
    QPixmap v_line;
};

#endif // RESOURCEMANAGER_H
