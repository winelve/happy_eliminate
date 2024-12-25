// ResourceManager.cpp
#include "resourcemanager.h"

ResourceManager::ResourceManager(){

    // 加载立方体纹理列表
    cube_texture_list_.push_back(QPixmap());
    cube_texture_list_.push_back(LoadSingleImg(":/cubes/bear/bear_cube.png"));
    cube_texture_list_.push_back(LoadSingleImg(":/cubes/cat/cat_cube.png"));
    cube_texture_list_.push_back(LoadSingleImg(":/cubes/chicken/chicken_cube.png"));
    cube_texture_list_.push_back(LoadSingleImg(":/cubes/fox/fox_cube.png"));
    cube_texture_list_.push_back(LoadSingleImg(":/cubes/frog/frog_cube.png"));
    cube_texture_list_.push_back(LoadSingleImg(":/cubes/horse/horse_cube.png"));


    destroy_texture_list_.push_back(LoadSingleImg(":/cubes/effect/kai_231_destroy_light_10.png"));
    destroy_texture_list_.push_back(LoadSingleImg(":/cubes/effect/kai_231_destroy_light_11.png"));
    destroy_texture_list_.push_back(LoadSingleImg(":/cubes/effect/kai_231_destroy_light_12.png"));
    destroy_texture_list_.push_back(LoadSingleImg(":/cubes/effect/kai_231_destroy_light_13.png"));
    destroy_texture_list_.push_back(LoadSingleImg(":/cubes/effect/kai_231_destroy_light_14.png"));
    destroy_texture_list_.push_back(LoadSingleImg(":/cubes/effect/kai_231_destroy_light_15.png"));

    h_line = QPixmap(":/cubes/effect/kai_231_bomb_white_h.png");
    v_line = QPixmap(":/cubes/effect/kai_231_bomb_white_v.png");

    magic_texture_list_ = LoadAtlas(":/cubes/bird/bird_select_30.png",30);
    magic_effect_texture_list_ = LoadAtlas(":/cubes/bird/bird_effect_20.png",20);

    word_effect_list_.push_back(LoadSingleImg(":/cubes/effect/good.png"));
    word_effect_list_.push_back(LoadSingleImg(":/cubes/effect/great.png"));
    word_effect_list_.push_back(LoadSingleImg(":/cubes/effect/execellent.png"));
    word_effect_list_.push_back(LoadSingleImg(":/cubes/effect/amazing.png"));
    word_effect_list_.push_back(LoadSingleImg(":/cubes/effect/unbelievable.png"));

    win_img_.push_back(LoadSingleImg(":/cubes/effect/win.png"));
    win_img_.push_back(LoadSingleImg(":/cubes/effect/next_level.png"));
    game_over_img_.push_back(LoadSingleImg(":/cubes/effect/gameover.png"));

    LoadAllAniResources();
}


// 获取单例实例
ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

QPixmap ResourceManager::LoadSingleImg(const QString &path){
    QPixmap pixmap(path);
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image from path:" << path;
    }
    return pixmap;
}

bool ResourceManager::ImportResource(int animal, int animType, const QString& path, int num)
{
    int resourceID = CalculateResourceID(animal, animType);

    // 检查资源是否已经导入
    if (ani_resources_.find(resourceID) != ani_resources_.end()) {
        qDebug() << "Resource already imported. ID:" << resourceID;
        return false;
    }

    // 加载Atlas图集并分割为帧
    std::vector<QPixmap> frames = LoadAtlas(path, num);
    if (frames.empty()) {
        qDebug() << "Failed to load atlas for ResourceID:" << resourceID << "Path:" << path;
        return false;
    }

    // 存储到资源映射中
    ani_resources_[resourceID] = frames;
    qDebug() << "Successfully imported ResourceID:" << resourceID << "from Path:" << path;
    return true;
}

std::vector<QPixmap> ResourceManager::GetAniResource(int animal, int animType) const
{

    int resourceID = CalculateResourceID(animal, animType);
    auto it = ani_resources_.find(resourceID);
    if (it != ani_resources_.end()) {
        return it->second;
    } else {
        qDebug() << "Resource not found. ID:" << resourceID;
        return {};
    }
}

int ResourceManager::CalculateResourceID(int animal, int animType) const
{
    return (static_cast<int>(animal) * 10) + static_cast<int>(animType);
}

std::vector<QPixmap> ResourceManager::LoadAtlas(const QString& path, int num)
{
    std::vector<QPixmap> atlas_list;

    // 检查帧数是否有效
    if (num <= 0) {
        qDebug() << "Error: Number of frames must be positive.";
        return atlas_list; // 返回空向量
    }

    // 加载精灵表图片
    QPixmap spriteSheet(path);
    if (spriteSheet.isNull()) {
        qDebug() << "Error: Failed to load image from path:" << path;
        return atlas_list; // 返回空向量
    }

    int totalWidth = spriteSheet.width();
    int height = spriteSheet.height();

    // 计算每一帧的宽度
    if (totalWidth % num != 0) {
        qDebug() << "Warning: The image width is not evenly divisible by the number of frames.";
    }
    int frameWidth = totalWidth / num;

    // 预分配空间以提高效率
    atlas_list.reserve(num);

    // 分割图片并存储到atlas_list
    for (int i = 0; i < num; ++i) {
        // 定义每一帧的矩形区域
        QRect frameRect(i * frameWidth, 0, frameWidth, height);
        // 如果是最后一帧，可能需要调整宽度以包含剩余的像素
        if (i == num - 1) {
            frameRect.setWidth(totalWidth - i * frameWidth);
        }

        // 拷贝每一帧
        QPixmap frame = spriteSheet.copy(frameRect);
        if (frame.isNull()) {
            qDebug() << "Error: Failed to copy frame" << i;
            continue; // 跳过当前帧
        }

        // 存储到atlas_list
        atlas_list.push_back(frame);
    }

    qDebug() << "Successfully loaded" << atlas_list.size() << "frames from" << path;
    return atlas_list;
}



// 导入所有资源
void ResourceManager::LoadAllAniResources()
{
    // 导入 Bear 动画
    ImportResource(AnimalType::Bear, AnimationType::Click, ":/cubes/bear/bear_click_30.png", 30);
    ImportResource(AnimalType::Bear, AnimationType::Col, ":/cubes/bear/bear_column_30.png", 30);
    ImportResource(AnimalType::Bear, AnimationType::Row, ":/cubes/bear/bear_line_30.png", 30);
    ImportResource(AnimalType::Bear, AnimationType::Wrap, ":/cubes/bear/bear_wrap_49.png", 49);

    // 导入 Cat 动画
    ImportResource(AnimalType::Cat, AnimationType::Click, ":/cubes/cat/cat_click_30.png", 30);
    ImportResource(AnimalType::Cat, AnimationType::Col, ":/cubes/cat/cat_column_29.png", 29);
    ImportResource(AnimalType::Cat, AnimationType::Row, ":/cubes/cat/cat_line_29.png", 29);
    ImportResource(AnimalType::Cat, AnimationType::Wrap, ":/cubes/cat/cat_wrap_49.png", 49);

    // 导入 Chicken 动画
    ImportResource(AnimalType::Chicken, AnimationType::Click, ":/cubes/chicken/chicken_click_30.png", 30);
    ImportResource(AnimalType::Chicken, AnimationType::Col, ":/cubes/chicken/chicken_column_29.png", 29);
    ImportResource(AnimalType::Chicken, AnimationType::Row, ":/cubes/chicken/chicken_line_29.png", 29);
    ImportResource(AnimalType::Chicken, AnimationType::Wrap, ":/cubes/chicken/chicken_wrap_49.png", 49);

    // 导入 Fox 动画
    ImportResource(AnimalType::Fox, AnimationType::Click, ":/cubes/fox/fox_click_29.png", 29);
    ImportResource(AnimalType::Fox, AnimationType::Col, ":/cubes/fox/fox_column_29.png", 29);
    ImportResource(AnimalType::Fox, AnimationType::Row, ":/cubes/fox/fox_line_29.png", 29);
    ImportResource(AnimalType::Fox, AnimationType::Wrap, ":/cubes/fox/fox_wrap_48.png", 48);

    // 导入 Frog 动画
    ImportResource(AnimalType::Frog, AnimationType::Click, ":/cubes/frog/frog_click_29.png", 29);
    ImportResource(AnimalType::Frog, AnimationType::Col, ":/cubes/frog/frog_column_28.png", 28);
    ImportResource(AnimalType::Frog, AnimationType::Row, ":/cubes/frog/frog_line_28.png", 28);
    ImportResource(AnimalType::Frog, AnimationType::Wrap, ":/cubes/frog/frog_wrap_49.png", 49);

    // 导入 Horse 动画
    ImportResource(AnimalType::Horse, AnimationType::Click, ":/cubes/horse/horse_click_30.png", 30);
    ImportResource(AnimalType::Horse, AnimationType::Col, ":/cubes/horse/horse_column_30.png", 30);
    ImportResource(AnimalType::Horse, AnimationType::Row, ":/cubes/horse/horse_line_30.png", 30);
    ImportResource(AnimalType::Horse, AnimationType::Wrap, ":/cubes/horse/horse_wrap_49.png", 49);

    // 您可以根据需要继续添加更多资源
}


QPixmap ResourceManager::GetWordEffect(int times){
    if(times > 7 || times < 2){
        qDebug() << "不存在该资源";
        return QPixmap();
    }
    int id = times - 2;
    return word_effect_list_[id];
}
