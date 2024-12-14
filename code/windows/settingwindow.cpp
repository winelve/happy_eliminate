#include "settingwindow.h"
#include "ui_settingwindow.h"
#include "mainwindow.h"

#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>

SettingWindow::SettingWindow(QWidget *parent)
    : FrameLessWindow(parent)
    , ui(new Ui::SettingWindow)
{
    ui->setupUi(this);
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
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    this->close();
}


void SettingWindow::on_btnMusic_clicked()
{
    if (music_status) {
        music_status = false;
        ui->btnMusic->setIcon(QIcon(":/gui/settingWindow/1-.png"));
    }else{
        music_status = true;
        ui->btnMusic->setIcon(QIcon(":/gui/settingWindow/1.png"));
    }
}


void SettingWindow::on_btnSound_clicked()
{
    if (sound_status) {
        sound_status = false;
        ui->btnSound->setIcon(QIcon(":/gui/settingWindow/3-.png"));
    }else{
        sound_status = true;
        ui->btnSound->setIcon(QIcon(":/gui/settingWindow/3.png"));
    }
}

