#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H


//所有事Entity的实体都应该进到这里面来
#include <QPainter>
#include <QString>
#include <vector>
#include <map>

#include "../entity/entity.h"

class RenderManager {
public:
    // 获取 RenderManager 单例
    static RenderManager& instance();

    // 禁止拷贝构造和赋值
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;

    // 添加 Entity 到指定分组
    void AddEntityToGroup(const QString &groupName, Entity *entity);
    // 添加多个 Entity 到指定分组
    void AddEntityToGroup(const QString &groupName, const std::vector<Entity*> &entities);
    // 从指定分组移除 Entity
    void RemoveEntityFromGroup(const QString &groupName, Entity *entity);
    // 清空指定分组
    void ClearGroup(const QString &groupName);
    // 检查特定分组的动画是否已经播放完毕
    bool IsGroupAnimationFinished(const QString &groupName);

    void ClearNullEntity();
    void RenderAll(QPainter &painter,int layer=0);    // 渲染所有分组的实体（根据层级排序）
    void UpdateAll(int deltaTime);       // 更新所有实体

    void Reset();

private:
    // 私有构造函数
    RenderManager() = default;

    std::map<QString, std::vector<Entity*>> groups_; // 键值对：分组名 -> 实体列表
};

#endif // RENDER_MANAGER_H
