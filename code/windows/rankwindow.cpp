#include "rankwindow.h"
#include "ui_rankwindow.h"

RankWindow::RankWindow(QWidget *parent)
    : FrameLessWindow(parent)
    , ui(new Ui::RankWindow)
{
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(900,675);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
}

RankWindow::~RankWindow(){delete ui;}
