#include "cvolumesliderdialog.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QEvent>

CVolumeSliderDialog::CVolumeSliderDialog(QWidget *parent)
    : QWidget{parent}
{
    // 设置为无边框窗口并去掉阴影
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true); // 设置透明背景
    initUI();  // 初始化UI
}

CVolumeSliderDialog::~CVolumeSliderDialog() {}

void CVolumeSliderDialog::setValue(int value)
{
    if(m_slider != nullptr)
    {
        m_slider->setValue(value);  // 设置滑块的值
    }
}


void CVolumeSliderDialog::initUI(){
    m_slider = new QSlider();
    m_slider->resize(370, 41);
    m_slider->setMaximum(100);
    m_slider->setMinimum(0);
    m_slider->setValue(0);
    m_slider->setStyleSheet("QSlider{background:transparent;}"
                            "QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
                            "QSlider::sub-page:vertical{background-color:rgba(196, 196, 196, 0.5);width:4px;border-radius:2px;}"
                            "QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
                            "QSlider::handle:vertical{height:20px;width:20px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                            "QSlider::handle:vertical:hover{height:20px;width:20px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                            "QSlider::handle:vertical:pressed{height:20px;width:20px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");

    connect(m_slider, &QSlider::valueChanged, this, &CVolumeSliderDialog::onVolumeChanged);
    // 设置布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);  // 去除边距
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_slider);
    setValue(10);
}

void CVolumeSliderDialog::onVolumeChanged()
{
    if(m_slider != nullptr)
    {
        int value = m_slider->value();

        if (value == 0)
        {
            m_slider->setStyleSheet("QSlider{background:transparent;}"
                                    "QSlider::add-page:vertical{background-color:rgba(0, 0, 0, 0.8);width:4px;border-radius:2px;}"
                                    "QSlider::sub-page:vertical{background-color:rgba(196, 196, 196, 0.5);width:4px;border-radius:2px;}"
                                    "QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
                                    "QSlider::handle:vertical{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                    "QSlider::handle:vertical:hover{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                    "QSlider::handle:vertical:pressed{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");
        }
        else if (value > 0 && value < 100)
        {
            m_slider->setStyleSheet("QSlider{background:transparent;}"
                                    "QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
                                    "QSlider::sub-page:vertical{background-color:rgba(196, 196, 196, 0.5);width:4px;border-radius:2px;}"
                                    "QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
                                    "QSlider::handle:vertical{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                    "QSlider::handle:vertical:hover{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                    "QSlider::handle:vertical:pressed{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");
        }
        else
        {
            m_slider->setStyleSheet("QSlider{background:transparent;}"
                                    "QSlider::add-page:vertical{background-color:#FFFFFF;width:4px;border-radius:2px;}"
                                    "QSlider::sub-page:vertical{background-color:rgba(0, 0, 0, 0.8);width:4px;border-radius:2px;}"
                                    "QSlider::groove:vertical{background:transparent;width:4px;border-radius:2px;}"
                                    "QSlider::handle:vertical{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                    "QSlider::handle:vertical:hover{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}"
                                    "QSlider::handle:vertical:pressed{height:20px;width:21px;border-image:url(:/gui/settingWindow/4.png);margin-top:0px;margin-bottom:0px;margin-left:-8px;margin-right:-8px;}");
        }

        emit signalVolumeChanged(value);
    }
}

void CVolumeSliderDialog::paintEvent(QPaintEvent *event)
{
    QStyleOption option;

    option.initFrom(this);
    QPainter p(this);

    style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);
}

bool CVolumeSliderDialog::event(QEvent *e)
{
    if (e->type() == QEvent::ActivationChange)
    {
        if (QApplication::activeWindow() != this)
        {
            this->close();
        }
    }

    return QWidget::event(e);
}
