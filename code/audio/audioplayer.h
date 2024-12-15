#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QMediaPlayer>


class AudioPlayer : public QObject {
    Q_OBJECT

public:

    ~AudioPlayer();

    // 播放全局背景音乐，循环播放
    void PlayBackgroundMusic(const QString &AudioFilePath);

    // 播放音效
    void PlaySoundEffect(const QString &AudioFilePath);

    void SetBackgroundMusicVolume(const int VolumeNumber);

    void SetSoundEffectVolume(const int VolumeNumber);

    // 提供一个静态方法来获取单例对象
    static AudioPlayer* getInstance() {
        if (!instance) {
            instance = new AudioPlayer();
        }
        return instance;
    }

private:

     explicit AudioPlayer();

    static AudioPlayer* instance; // 静态私有成员变量，存储单例对象

    QMediaPlayer *BackgroundMusicPlayer;
    QAudioOutput *BackgroundMusicOutput;

    QMediaPlayer *SoundEffectPlayer;
    QAudioOutput *SoundEffectOutput;

signals:

public slots:
};

#endif // AUDIOPLAYER_H
