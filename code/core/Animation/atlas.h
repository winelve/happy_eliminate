#ifndef ATLAS_H
#define ATLAS_H


#include <QString>
#include <vector>
#include <QPixmap>

class Atlas
{
public:
    Atlas();

    void Load(QString path,int num);
    void Clear();
    int GetSize();
    QPixmap* GetFrame(int idx);
    void AddFrame(const QPixmap &frame);
    bool Empty() { return !GetSize(); }

private:
    std::vector<QPixmap> frame_list_;

};

#endif // ATLAS_H
