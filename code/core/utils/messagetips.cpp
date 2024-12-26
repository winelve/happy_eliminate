#include "messagetips.h"

#include <QScreen>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include<QApplication>
#include<QDebug>

MessageTips::MessageTips(QString showStr,QWidget *parent) : QWidget(parent),
    hBoxlayout(new QHBoxLayout(this)),
    mText(new QLabel(this)),
    backgroundColor(QColor(120,120,120)),
    textColor(QColor(255,255,255)),
    textSize(14),
    showTime(1000),
    closeTime(100),
    closeSpeed(0.1),
    frameColor(QColor(0,0,0,0)),
    frameSize(0),
    opacityValue(1.0)
{
    setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool|Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground); // ****这里很重要****
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);// 禁止鼠标事件
    this->setAttribute(Qt::WA_DeleteOnClose,true);//关闭窗口自动调用析构函数
    this->showStr = showStr;
    hBoxlayout->addWidget(mText);
    InitLayout();
}

MessageTips::~MessageTips()
{
    if (hBoxlayout != NULL)
    {
        delete hBoxlayout;
        hBoxlayout = NULL;
    }
    if (mText != NULL)
    {
        delete mText;
        mText = NULL;
    }
}

void MessageTips::InitLayout()
{
    this->setWindowOpacity(opacityValue);

    //文字显示居中，设置字体，大小，颜色
    QFont font = QFont("微软雅黑",textSize,QFont::Bold);
    mText->setStyleSheet("QLabel{background-color:transparent;}");
    mText->setFont(font);
    mText->setText(showStr);
    mText->adjustSize();
    mText->setAlignment(Qt::AlignCenter);
    QPalette label_pe;//设置字体颜色
    label_pe.setColor(QPalette::WindowText, textColor);
    mText->setPalette(label_pe);

    QTimer *mtimer = new QTimer(this);//隐藏的定时器
    mtimer->setTimerType(Qt::PreciseTimer);
    connect(mtimer,&QTimer::timeout,this,[=](){
        if(opacityValue<=0){ this->close(); }
        opacityValue = opacityValue-closeSpeed;
        this->setWindowOpacity(opacityValue);    //设置窗口透明度
    });

    QTimer *mShowtimer = new QTimer(this);//显示时间的定时器
    mShowtimer->setTimerType(Qt::PreciseTimer);// 修改定时器对象的精度
    connect(mShowtimer,&QTimer::timeout,this,[=](){
        mtimer->start(closeTime);//执行延时自动关闭
        mtimer->destroyed();
        mShowtimer->deleteLater();
    });
    mShowtimer->start(showTime);

    //设置屏幕居中显示
    // 获取屏幕几何信息
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // 移动窗口到屏幕底部居中位置
    this->move(
        (screenGeometry.width() - mText->width()) / 2,          // 水平居中
        (screenGeometry.height() - mText->height()) * 2 / 8     // 垂直位置在屏幕底部偏上
        );

    // 设置窗口透明以允许鼠标事件穿透
    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void MessageTips::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBrush(QBrush(backgroundColor));//窗体的背景色

    painter.setPen(QPen(frameColor,frameSize));//窗体边框的颜色和笔画大小
    QRectF rect(0, 0, this->width(), this->height());
    painter.drawRoundedRect(rect, 15, 15); // round rect
}

void MessageTips::setBackgroundColor(const QColor &value)
{
    backgroundColor = value;
}

void MessageTips::setTextColor(const QColor &value)
{
    textColor = value;
    InitLayout();
}

void MessageTips::setTextSize(int value)
{
    textSize = value;
    InitLayout();
}

void MessageTips::setShowTime(int value)
{
    showTime = value;
    InitLayout();
}

void MessageTips::setCloseTimeSpeed(int closeTime, double closeSpeed)
{
    if(closeSpeed>0 && closeSpeed<=1){
        this->closeSpeed = closeSpeed;
    }
    this->closeTime = closeTime;
    InitLayout();
}

void MessageTips::setFrameColor(const QColor &value)
{
    frameColor = value;
}

void MessageTips::setFrameSize(int value)
{
    frameSize = value;
}

void MessageTips::setOpacityValue(double value)
{
    opacityValue = value;
    InitLayout();
}
