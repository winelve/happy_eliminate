#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QObject>
#include<QSettings>

class AudioControl : public QObject
{
    Q_OBJECT
public:
    // 得到AudioControl单例实例
    static AudioControl* instance();

    // 音乐状态的 getter 和 setter
    bool musicStatus() const;
    void setMusicStatus(bool status);

    // 音效状态的 getter 和 setter
    bool soundStatus() const;
    void setSoundStatus(bool status);

    // 音乐音量的 getter 和 setter
    int musicVolume() const;
    void setMusicVolume(int volume);

    // 音效音量的 getter 和 setter
    int soundVolume() const;
    void setSoundVolume(int volume);

    void saveSettings();
    void loadSettings();

private:
    explicit AudioControl(QObject *parent = nullptr);
    ~AudioControl();

    static AudioControl* m_instance;

    bool m_musicStatus;
    bool m_soundStatus;
    int m_musicVolume;
    int m_soundVolume;

    QSettings m_settings;
};

#endif // AUDIOCONTROL_H
