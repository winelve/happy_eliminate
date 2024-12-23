#ifndef POSMANAGER_H
#define POSMANAGER_H

#include <QObject>
#include "./utils/vector2.h"

class PosManager : public QObject
{
    Q_OBJECT

public:
    static PosManager* instance();


    void HandleClick(const Vector2 &pos);  // 处理点击事件的逻辑
    bool Available() { return available_; }
    void Reselect(); //重置选选择的内容
    Vector2 GetPos1() { return pos1_; }
    Vector2 GetPos2() { return pos2_; }

    bool IsChoosed()  { return choosed_; } //选中
private:
    explicit PosManager(QObject *parent = nullptr);

    bool choosed_;
    Vector2 pos1_;
    Vector2 pos2_;
    bool available_; //是否可用




    // 禁止复制和赋值
    PosManager(const PosManager&) = delete;
    PosManager& operator=(const PosManager&) = delete;
signals:
    void finish_choose_pos_singal();
};

#endif // POSMANAGER_H

