#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QSettings>

class ModeManager : public QObject
{
    Q_OBJECT

public:
    static ModeManager* getinstance();
    // checkerboard 的 getter 和 setter
    int getCheckerboard() const;
    void setCheckerboard(int status);

    // difficulty 的 getter 和 setter
    int getDifficulty() const;
    void setDifficulty(int status);

    // play 的 getter 和 setter
    int getPlay() const;
    void setPlay(int status);

    void saveSettings();
    void loadSettings();

private:
    explicit ModeManager(QObject *parent = nullptr); // 私有构造函数，防止外部创建实例
    ~ModeManager();
    static ModeManager* m_instance;

    QSettings m_settings;
    int select_checkerboard;  // 选择棋盘大小
    int select_difficulty;    // 选择游戏难度（棋子种类个数）
    int select_play;          // 选择玩法
};

#endif // MODEMANAGER_H
