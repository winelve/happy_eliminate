#include "settingwindow.h"
#include "ui_settingwindow.h"
#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>
#include "audiocontrol.h"

SettingWindow::SettingWindow(QWidget *parent)
    : FrameLessWindow(parent)
    , ui(new Ui::SettingWindow)
{
    ui->setupUi(this);

    // 设置滑动条值
    ui->music_slider->setValue(audioCtrl->musicVolume());
    ui->sound_slider->setValue(audioCtrl->soundVolume());

    // 设置按钮状态
    if (audioCtrl->musicStatus()) {
        ui->btnMusic->setIcon(QIcon(":/gui/settingWindow/1.png"));
    } else {
        ui->btnMusic->setIcon(QIcon(":/gui/settingWindow/1-.png"));
    }

    if (audioCtrl->soundStatus()) {
        ui->btnSound->setIcon(QIcon(":/gui/settingWindow/3.png"));
    } else {
        ui->btnSound->setIcon(QIcon(":/gui/settingWindow/3-.png"));
    }

    // 创建阴影效果
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect;
    shadow1->setBlurRadius(15); // 阴影模糊半径
    shadow1->setOffset(3, 3); // 阴影偏移量
    shadow1->setColor(QColor(191, 142, 26, 130)); // 阴影颜色

    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect;
    shadow2->setBlurRadius(15); // 阴影模糊半径
    shadow2->setOffset(3, 3); // 阴影偏移量
    shadow2->setColor(QColor(191, 142, 26, 130)); // 阴影颜色

    // 将阴影效果应用到按钮
    ui->btnMusic->setGraphicsEffect(shadow1);
    ui->btnSound->setGraphicsEffect(shadow2);

    // 设置字体
    int fontId = QFontDatabase::addApplicationFont(":/font/font.ttf");
    if (fontId == -1) {
        qWarning() << "没有找到字体！";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        ui->label_music->setFont(font);
        ui->label_music->setStyleSheet(
            "QLabel { "
            "font-size: 21px; "
            "color: #aa7743 ; "
            "}"
            );
        ui->label_sound->setFont(font);
        ui->label_sound->setStyleSheet(
            "QLabel { "
            "font-size: 21px; "
            "color: #aa7743 ; "
            "}"
            );
        ui->label->setFont(font);
        ui->label->setStyleSheet(
            "QLabel { "
            "font-size: 26px; "
            "color: #508330; "
            "}"
            );
    }
}

SettingWindow::~SettingWindow()
{
    delete ui;
}

void SettingWindow::on_btnReturn_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    delay(150);
    this->close();
}


void SettingWindow::on_btnMusic_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    if (audioCtrl->musicStatus()) {
        audioCtrl->setMusicStatus(false);
        audioCtrl->setMusicVolume(0);
        audioPlayer->SetBackgroundMusicVolume(0);
        ui->music_slider->setValue(0);
        ui->btnMusic->setIcon(QIcon(":/gui/settingWindow/1-.png"));
    }else{
        audioCtrl->setMusicStatus(true);
        audioCtrl->setMusicVolume(50);
        audioPlayer->SetBackgroundMusicVolume(50);
        ui->music_slider->setValue(50);
        ui->btnMusic->setIcon(QIcon(":/gui/settingWindow/1.png"));
    }
}


void SettingWindow::on_btnSound_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    if (audioCtrl->soundStatus()) {
        audioCtrl->setSoundStatus(false);
        audioCtrl->setSoundVolume(0);
        audioPlayer->SetSoundEffectVolume(0);
        ui->sound_slider->setValue(0);
        ui->btnSound->setIcon(QIcon(":/gui/settingWindow/3-.png"));
    }else{
        audioCtrl->setSoundStatus(true);
        audioCtrl->setSoundVolume(50);
        audioPlayer->SetSoundEffectVolume(50);
        ui->sound_slider->setValue(50);
        ui->btnSound->setIcon(QIcon(":/gui/settingWindow/3.png"));
    }
}

void SettingWindow::on_music_slider_valueChanged(int value)
{
        // 如果音量为零，更新图标为关闭状态
    if (value == 0) {
        audioCtrl->setMusicStatus(false);
        ui->btnMusic->setIcon(QIcon(":/gui/settingWindow/1-.png"));
    } else {
        // 如果音量大于零，更新图标为开启状态
        audioCtrl->setMusicStatus(true);
        ui->btnMusic->setIcon(QIcon(":/gui/settingWindow/1.png"));
    }
    audioCtrl->setMusicVolume(value);
    audioPlayer->SetBackgroundMusicVolume(value);
}


void SettingWindow::on_sound_slider_valueChanged(int value)
{
    // 如果音量为零，更新图标为关闭状态
    if (value == 0) {
        audioCtrl->setSoundStatus(false);
        ui->btnSound->setIcon(QIcon(":/gui/settingWindow/3-.png"));
    } else {
        // 如果音量大于零，更新图标为开启状态
        audioCtrl->setSoundStatus(true);
        ui->btnSound->setIcon(QIcon(":/gui/settingWindow/3.png"));
    }
    audioCtrl->setSoundVolume(value);
    audioPlayer->SetSoundEffectVolume(value);
}

