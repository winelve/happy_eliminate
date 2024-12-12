#include "audioplayer.h"
#include <QAudioOutput>

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent) {
    backgroundMusicPlayer = new QMediaPlayer(this);
    soundEffectPlayer = new QMediaPlayer(this);
}

AudioPlayer::~AudioPlayer() {
    backgroundMusicPlayer->stop();
    soundEffectPlayer->stop();
}

void AudioPlayer::playBackgroundMusic(const QString &audioFilePath) {
    backgroundMusicPlayer->setSource(QUrl::fromLocalFile(audioFilePath));

    backgroundMusicPlayer->setLoops(true); // 循环播放
    backgroundMusicPlayer->play();

}

void AudioPlayer::playSoundEffect(const QString &audioFilePath) {
    if (soundEffectPlayer->source().isValid()) {
        soundEffectPlayer->stop();
    }
    soundEffectPlayer->setSource(QUrl::fromLocalFile(audioFilePath));
    QAudioOutput *audioOutput = new QAudioOutput(this);
    soundEffectPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(100); // 设置音量，范围是0-100
    connect(soundEffectPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            soundEffectPlayer->deleteLater();
        }
    });
    soundEffectPlayer->play();
}
