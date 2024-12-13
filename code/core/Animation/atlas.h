#ifndef ATLAS_H
#define ATLAS_H


#include <QString>
#include <vector>
#include <QPixmap>

//资源管理器,用于存储所有的图集资源
class Atlas
{
public:
    Atlas();

    void Load(const QString &path,int num);
    void Clear();
    int GetSize();
    QPixmap* GetAtlas(int idx);
    void AddAtlas(const QPixmap &frame);
    bool Empty() { return !GetSize(); }

private:
    std::vector<QPixmap> atlas_list;
};

#endif // ATLAS_H
