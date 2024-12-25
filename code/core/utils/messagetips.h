#ifndef MESSAGETIPS_H
#define MESSAGETIPS_H

#include <QWidget>

class QHBoxLayout;
class QLabel;
class MessageTips : public QWidget
{
    Q_OBJECT
public:
    explicit MessageTips(QString showStr="默认显示", QWidget *parent = nullptr);
    ~MessageTips();
    //设置背景颜色
    void setBackgroundColor(const QColor &value);
    //设置文本颜色
    void setTextColor(const QColor &value);
    //设置文本字体大小
    void setTextSize(int value);
    //设置显示时间
    void setShowTime(int value);
    //设置关闭速度
    void setCloseTimeSpeed(int closeTime = 100,double closeSpeed = 0.1);
    //设置边框颜色
    void setFrameColor(const QColor &value);
    //设置边框粗细
    void setFrameSize(int value);
    //设置透明度
    void setOpacityValue(double value);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void InitLayout();//初始化窗体的布局和部件
    QHBoxLayout *hBoxlayout;//布局显示控件布局
    QLabel *mText;//用于显示文字的控件
    QString showStr;//显示的字符串

    QColor backgroundColor;//窗体的背景色
    QColor textColor;//字体颜色
    int textSize;//显示字体大小
    int showTime;//显示时间
    int closeTime;//关闭需要时间
    double closeSpeed;//窗体消失的平滑度，大小0~1
    QColor frameColor;//边框颜色
    int frameSize;//边框粗细大小
    double opacityValue;//窗体初始化透明度
};

#endif // MESSAGETIPS_H
