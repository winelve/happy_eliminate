#include "modeselectionwindow.h"
#include "ui_modeselectionwindow.h"
#include "mainwindow.h"

#include <QFontDatabase>

ModeSelectionWindow::ModeSelectionWindow(QWidget *parent)
    : FrameLessWindow(parent)
    , ui(new Ui::ModeSelectionWindow)
{
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(900,675);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    //初始显示在难度选择上面
    ui->stackedWidget->setCurrentWidget(ui->page);

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

}

ModeSelectionWindow::~ModeSelectionWindow(){delete ui;}

void ModeSelectionWindow::on_btnReturn_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    delay(150);
    this->close();
}

void ModeSelectionWindow::on_btnDifficulty_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    if (selectionStatus != 1) {
        ui->btnDifficulty->setIcon(QIcon(":/gui/modeselectionWindow/1-.png"));
        ui->btnSize->setIcon(QIcon(":/gui/modeselectionWindow/2.png"));
        ui->btnPlay->setIcon(QIcon(":/gui/modeselectionWindow/3.png"));
        ui->stackedWidget->setCurrentWidget(ui->page);
        selectionStatus = 1;
    }
}

void ModeSelectionWindow::on_btnPlay_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    if (selectionStatus != 2) {
        ui->btnPlay->setIcon(QIcon(":/gui/modeselectionWindow/3-.png"));
        ui->btnSize->setIcon(QIcon(":/gui/modeselectionWindow/2.png"));
        ui->btnDifficulty->setIcon(QIcon(":/gui/modeselectionWindow/1.png"));
        ui->stackedWidget->setCurrentWidget(ui->page_2);
        selectionStatus = 2;
    }
}


void ModeSelectionWindow::on_btnSize_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    if (selectionStatus != 3) {
        ui->btnSize->setIcon(QIcon(":/gui/modeselectionWindow/2-.png"));
        ui->btnPlay->setIcon(QIcon(":/gui/modeselectionWindow/3.png"));
        ui->btnDifficulty->setIcon(QIcon(":/gui/modeselectionWindow/1.png"));
        ui->stackedWidget->setCurrentWidget(ui->page_3);
        selectionStatus = 3;
    }
}



// 6*6
void ModeSelectionWindow::on_btn6_clicked()
{
    audioPlayer->PlaySoundEffect("swap.mp3");
    if (selection != 1) {
        ui->btn6->setIcon(QIcon(":/gui/modeselectionWindow/0-.png"));
        ui->btn10->setIcon(QIcon(":/gui/modeselectionWindow/0.png"));
        ui->btn14->setIcon(QIcon(":/gui/modeselectionWindow/0.png"));
        selection = 1;
    }
}

// 10*10
void ModeSelectionWindow::on_btn10_clicked()
{
    audioPlayer->PlaySoundEffect("swap.mp3");
    if (selection != 2) {
        ui->btn10->setIcon(QIcon(":/gui/modeselectionWindow/0-.png"));
        ui->btn6->setIcon(QIcon(":/gui/modeselectionWindow/0.png"));
        ui->btn14->setIcon(QIcon(":/gui/modeselectionWindow/0.png"));
        selection = 2;
    }
}

// 14*14
void ModeSelectionWindow::on_btn14_clicked()
{
    audioPlayer->PlaySoundEffect("swap.mp3");
    if (selection != 3) {
        ui->btn14->setIcon(QIcon(":/gui/modeselectionWindow/0-.png"));
        ui->btn6->setIcon(QIcon(":/gui/modeselectionWindow/0.png"));
        ui->btn10->setIcon(QIcon(":/gui/modeselectionWindow/0.png"));
        selection = 3;
    }
}

// 闯关模式
void ModeSelectionWindow::on_btnBreak_clicked()
{
    audioPlayer->PlaySoundEffect("swap.mp3");
}

// 限时模式
void ModeSelectionWindow::on_btnHeart_clicked()
{
    audioPlayer->PlaySoundEffect("swap.mp3");
}

// 简单模式
void ModeSelectionWindow::on_btnEasy_clicked()
{
    audioPlayer->PlaySoundEffect("swap.mp3");
}

// 困难模式
void ModeSelectionWindow::on_btnDifficult_clicked()
{
    audioPlayer->PlaySoundEffect("swap.mp3");
}

// 极限模式
void ModeSelectionWindow::on_btnExtreme_clicked()
{
    audioPlayer->PlaySoundEffect("swap.mp3");
}

