#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QMediaPlayer>


class AudioPlayer : public QObject {
    Q_OBJECT

public:

    explicit AudioPlayer();

    ~AudioPlayer();

    // 播放全局背景音乐，循环播放
    void PlayBackgroundMusic(const QString &AudioFilePath);

    // 播放音效
    void PlaySoundEffect(const QString &AudioFilePath);

    void SetBackgroundMusicVolume(int VolumeNumber);

    void SetSoundEffectVolume(int VolumeNumber);

private:
    QMediaPlayer *BackgroundMusicPlayer;
    QAudioOutput *BackgroundMusicOutput;

    QMediaPlayer *SoundEffectPlayer;
    QAudioOutput *SoundEffectOutput;

signals:

public slots:
};

#endif // AUDIOPLAYER_H
