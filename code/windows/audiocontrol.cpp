#include "audiocontrol.h"

AudioControl* AudioControl::m_instance = nullptr;

AudioControl::AudioControl(QObject *parent)
    : QObject(parent),
    m_settings("MyApp", "Settings"),
    m_musicStatus(true),  // 默认音乐开启
    m_soundStatus(true),  // 默认音效开启
    m_musicVolume(50),    // 默认音乐音量
    m_soundVolume(50)     // 默认音效音量
{
    loadSettings();
}

AudioControl::~AudioControl() {
    saveSettings();
}

// 得到AudioControl单例实例
AudioControl* AudioControl::instance() {
    if (!m_instance) {
        m_instance = new AudioControl();
    }
    return m_instance;
}

bool AudioControl::musicStatus() const {
    return m_musicStatus;
}

void AudioControl::setMusicStatus(bool status) {
    if (m_musicStatus != status) {
        m_musicStatus = status;
        saveSettings();
    }
}

bool AudioControl::soundStatus() const {
    return m_soundStatus;
}

void AudioControl::setSoundStatus(bool status) {
    if (m_soundStatus != status) {
        m_soundStatus = status;
        saveSettings();
    }
}

int AudioControl::musicVolume() const {
    return m_musicVolume;
}

void AudioControl::setMusicVolume(int volume) {
    if (m_musicVolume != volume) {
        m_musicVolume = volume;
        saveSettings();
    }
}

int AudioControl::soundVolume() const {
    return m_soundVolume;
}

void AudioControl::setSoundVolume(int volume) {
    if (m_soundVolume != volume) {
        m_soundVolume = volume;
        saveSettings();
    }
}

// 保存设置到 QSettings
void AudioControl::saveSettings() {
    m_settings.setValue("musicStatus", m_musicStatus);
    m_settings.setValue("soundStatus", m_soundStatus);
    m_settings.setValue("musicVolume", m_musicVolume);
    m_settings.setValue("soundVolume", m_soundVolume);
}

// 从 QSettings 加载设置
void AudioControl::loadSettings() {
    m_musicStatus = m_settings.value("musicStatus", true).toBool();
    m_soundStatus = m_settings.value("soundStatus", true).toBool();
    m_musicVolume = m_settings.value("musicVolume", 50).toInt();
    m_soundVolume = m_settings.value("soundVolume", 50).toInt();
}
