#include <QGraphicsDropShadowEffect>
#include <QTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "settingwindow.h"
#include "rankwindow.h"
#include "gamewidget.h"
#include "modeselectionwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : FrameLessWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(900,675);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);

    audioPlayer->PlayBackgroundMusic("worldscenebgm.mp3");

    // 创建阴影效果
    QGraphicsDropShadowEffect *shadow1 = new QGraphicsDropShadowEffect;
    shadow1->setBlurRadius(15); // 阴影模糊半径
    shadow1->setOffset(5, 5); // 阴影偏移量
    shadow1->setColor(QColor(0, 0, 0, 160)); // 阴影颜色

    QGraphicsDropShadowEffect *shadow2 = new QGraphicsDropShadowEffect;
    shadow2->setBlurRadius(15); // 阴影模糊半径
    shadow2->setOffset(5, 5); // 阴影偏移量
    shadow2->setColor(QColor(0, 0, 0, 160)); // 阴影颜色

    // 将阴影效果应用到按钮
    ui->btnAbout->setGraphicsEffect(shadow1);
    ui->btnSetting->setGraphicsEffect(shadow2);
}

MainWindow::~MainWindow(){delete ui;}

void delay(int x) {
    QTime dieTime = QTime::currentTime().addMSecs(x);
    while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::showMainWindow()
{
    this->show();  // 显示 MainWindow
}

//关于
void MainWindow::on_btnAbout_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    AboutWindow *aw = new AboutWindow();
    aw->move(this->pos().x(), this->pos().y());
    aw->show();
    delay(150);
    this->close();
}

//设置
void MainWindow::on_btnSetting_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    SettingWindow *sw = new SettingWindow();
    sw->move(this->pos().x(), this->pos().y());
    sw->show();
    delay(150);
    this->close();
}

//排行榜
void MainWindow::on_btnRank_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    RankWindow *rw = new RankWindow();
    rw->move(this->pos().x(), this->pos().y());
    rw->show();
    delay(150);
    this->close();
}


//退出
void MainWindow::on_btnQuit_clicked()
{
    this->close();
}

// 模式选择
void MainWindow::on_btnMode_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    ModeSelectionWindow *mw = new ModeSelectionWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    this->close();
}


void MainWindow::on_btnGame_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    RenderManager::instance();
    BoardManager::instance().ResetManager();
    // DataResource::instance()->reset_all_data();
    RenderManager::instance().Reset();


    GameWidget* gw = new GameWidget();
    gw->move(this->pos().x(), this->pos().y());
    gw->show();
    delay(20);
    this->close();
}

