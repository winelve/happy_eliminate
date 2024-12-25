#include "modemanager.h"

ModeManager* ModeManager::m_instance = nullptr;

ModeManager::ModeManager(QObject *parent)
    : QObject(parent),
    m_settings("MyApp", "Settings"),
    select_checkerboard(0),
    select_difficulty(0),
    select_play(0)
{
    loadSettings();
}

ModeManager::~ModeManager()
{
     saveSettings();
}


ModeManager *ModeManager::getinstance()
{
    if (!m_instance) {
        m_instance = new ModeManager();
    }
    return m_instance;
}

int ModeManager::getCheckerboard() const {
    return select_checkerboard;
}

void ModeManager::setCheckerboard(int status) {
    select_checkerboard = status;
    saveSettings();
}

int ModeManager::getDifficulty() const {
    return select_difficulty;
}

void ModeManager::setDifficulty(int status) {
    select_difficulty = status;
    saveSettings();
}

int ModeManager::getPlay() const {
    return select_play;
}

void ModeManager::setPlay(int status) {
    select_play = status;
    saveSettings();
}

// 保存设置到 QSettings
void ModeManager::saveSettings() {
    m_settings.setValue("checkerboard", select_checkerboard);  // 保存棋盘大小
    m_settings.setValue("difficulty", select_difficulty);      // 保存游戏难度
    m_settings.setValue("play", select_play);                  // 保存玩法选择
}

// 从 QSettings 加载设置
void ModeManager::loadSettings() {
    select_checkerboard = m_settings.value("checkerboard",0).toInt(); // 加载棋盘大小，默认值为 0
    select_difficulty = m_settings.value("difficulty",0).toInt();     // 加载游戏难度，默认值为 0
    select_play = m_settings.value("play",0).toInt();                 // 加载玩法选择，默认值为 0
}
