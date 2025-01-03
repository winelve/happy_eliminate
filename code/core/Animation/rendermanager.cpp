#include "rendermanager.h"
#include "../entity/cube.h"
#include <algorithm>

// 获取 RenderManager 单例
RenderManager& RenderManager::instance() {
    static RenderManager instance; // C++11 线程安全的静态局部变量
    return instance;
}

void RenderManager::AddEntityToGroup(const QString &groupName, Entity *entity) {
    groups_[groupName].push_back(entity);
    // qDebug() << "Render:size:" << groups_["cube"].size();
}

void RenderManager::AddEntityToGroup(const QString &groupName, const std::vector<Entity*> &entities) {
    // 将传入的 vector 追加到分组中
    groups_[groupName].insert(groups_[groupName].end(), entities.begin(), entities.end());
}

void RenderManager::RemoveEntityFromGroup(const QString &groupName, Entity *entity) {
    if (groups_.find(groupName) != groups_.end()) {
        auto &entities = groups_[groupName];
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
        if (entities.empty()) {
            groups_.erase(groupName); // 如果分组为空，则移除分组
        }
    }
}

void RenderManager::ClearGroup(const QString &groupName) {
    groups_.erase(groupName);
}

void RenderManager::RenderAll(QPainter &painter, int layer) {
    // 临时容器，用于存储指定层级的实体
    std::vector<Entity*> selectedEntities;

    // 遍历所有分组，筛选出指定层级的实体
    for (auto &[groupName, entities] : groups_) {
        for (Entity *entity : entities) {
            if (entity && entity->GetLayer() == layer) {
                selectedEntities.push_back(entity);
            }
        }
    }
    // 渲染所有符合条件的实体
    for (Entity *entity : selectedEntities) {
        if (entity) {
            entity->render(painter);
        }
    }
}

void RenderManager::UpdateAll(int deltaTime) {
    //先清除所有的失效的对象

    ClearNullEntity();
    // qDebug() << "Render:size:" << groups_["destroy"].size();
    //下面是更新逻辑
    for (auto &[groupName, entities] : groups_) {
        for (Entity *entity : entities) {
            if (entity) {
                entity->update(deltaTime);
            }
        }
    }

}

bool RenderManager::IsGroupAnimationFinished(const QString &groupName) {
    if (groups_.find(groupName) != groups_.end()) {
        for (Entity *entity : groups_[groupName]) {
            if (entity && !entity->IsFinished()) { // 假设 Entity 类中有 IsAnimationFinished 方法
                return false;
            }
        }
        return true; // 如果所有实体的动画都完成，返回 true
    }
    return false; // 分组不存在时返回 false
}


void RenderManager::ClearNullEntity() {
    for (auto &[groupName, entities] : groups_) {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                           [](Entity* entity) {
                               // 检查是否为空指针
                               if (entity == nullptr || entity->IsFinished()) {
                                   return true;
                               }

                               // 检查是否为 Cube 类型且 GetType 为 0
                               if (Cube* cube = dynamic_cast<Cube*>(entity)) {
                                   if (cube->GetType() == 0) {
                                       return true;
                                   }
                               }

                               return false;
                           }
                           ),
            entities.end()
            );
    }
}


void RenderManager::Reset()
{
    // 遍历所有分组并清空实体
    for (auto& group : groups_) {
        for (Entity* entity : group.second) {
            delete entity; // 释放动态分配的实体内存
        }
        group.second.clear(); // 清空分组内的实体列表
    }

    // 清空所有分组
    groups_.clear();

    qDebug() << "RenderManager has been reset.";
}
