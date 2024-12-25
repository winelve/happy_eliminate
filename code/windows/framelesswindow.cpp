#include "framelesswindow.h"
#include "ui_framelesswindow.h"

FrameLessWindow::FrameLessWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FrameLessWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);   //设置窗口无边
}

FrameLessWindow::~FrameLessWindow()
{
    delete ui;
}
