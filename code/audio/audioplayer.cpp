#include "code/audio/audioplayer.h"
#include <QAudioOutput>
#include <QDebug>

AudioPlayer::AudioPlayer(){


    BackgroundMusicPlayer = new QMediaPlayer(this);
    SoundEffectPlayer =new QMediaPlayer(this);

    SoundEffectOutput=new QAudioOutput(this);
    BackgroundMusicOutput=new QAudioOutput(this);



}

AudioPlayer::~AudioPlayer() {
    BackgroundMusicPlayer->stop();
    SoundEffectPlayer->stop();

}

void AudioPlayer::PlayBackgroundMusic(const QString &audioFilePath) {


    BackgroundMusicPlayer->setSource(QUrl("qrc:/audios/"+audioFilePath));
    BackgroundMusicPlayer->setAudioOutput(BackgroundMusicOutput);
    BackgroundMusicPlayer->setLoops(-1); // 循环播放
    BackgroundMusicPlayer->play();

}

void AudioPlayer::PlaySoundEffect(const QString &audioFilePath) {


    SoundEffectPlayer->setSource(QUrl("qrc:/audios/"+audioFilePath));
    SoundEffectPlayer->setAudioOutput(SoundEffectOutput);
    SoundEffectPlayer->setLoops(1);
    SoundEffectPlayer->play();

}

void AudioPlayer::SetBackgroundMusicVolume(int VolumeNumber){
    BackgroundMusicOutput->setVolume((0.01*VolumeNumber));
}

void AudioPlayer::SetSoundEffectVolume(int VolumeNumber){
    SoundEffectOutput->setVolume((0.01*VolumeNumber));
}
