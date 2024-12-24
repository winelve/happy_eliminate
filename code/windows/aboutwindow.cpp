#include "aboutwindow.h"
#include "ui_aboutwindow.h"

#include "mainwindow.h"
#include <QFontDatabase>

AboutWindow::AboutWindow(QWidget *parent)
    : FrameLessWindow(parent)
    , ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(900,675);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);

    // 设置字体
    int fontId = QFontDatabase::addApplicationFont(":/font/font.ttf");
    if (fontId == -1) {
        qWarning() << "没有找到字体！";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        ui->label->setFont(font);
        ui->label->setStyleSheet(
            "QLabel { "
            "font-size: 26px; "
            "color: #508330; "
            "}"
            );
    }
    //设置帮助显示区域 scrollArea
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 禁用水平滚动条

}

AboutWindow::~AboutWindow(){delete ui;}

void AboutWindow::on_btnReturn_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    delay(150);
    this->close();
}

