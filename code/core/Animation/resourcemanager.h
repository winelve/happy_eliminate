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
    QPixmap GetCube(int type) { return cube_texture_list_[type]; }

    std::vector<QPixmap> GetDestroyEffect() { return destroy_texture_list_; }
    QPixmap GetHLine() { return h_line; }
    QPixmap GetVLine() { return v_line; }

    // 放大3倍


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
    QPixmap h_line;
    QPixmap v_line;
};

#endif // RESOURCEMANAGER_H




















/*
// 静态常量定义资源ID
// Bear Animations
const static int bear_click_Ani = 11;
const static int bear_col_Ani = 12;
const static int bear_row_Ani = 13;
const static int bear_wrap_Ani = 14;

// Cat Animations
const static int cat_click_Ani = 21;
const static int cat_col_Ani = 22;
const static int cat_row_Ani = 23;
const static int cat_wrap_Ani = 24;

// Chicken Animations
const static int chicken_click_Ani = 31;
const static int chicken_col_Ani = 32;
const static int chicken_row_Ani = 33;
const static int chicken_wrap_Ani = 34;

// Fox Animations
const static int fox_click_Ani = 41;
const static int fox_col_Ani = 42;
const static int fox_row_Ani = 43;
const static int fox_wrap_Ani = 44;

// Frog Animations
const static int frog_click_Ani = 51;
const static int frog_col_Ani = 52;
const static int frog_row_Ani = 53;
const static int frog_wrap_Ani = 54;

// Horse Animations
const static int horse_click_Ani = 61;
const static int horse_col_Ani = 62;
const static int horse_row_Ani = 63;
const static int horse_wrap_Ani = 64;
*/
