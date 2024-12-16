#ifndef CVOLUMESLIDERDIALOG_H
#define CVOLUMESLIDERDIALOG_H

#include <QWidget>
#include <QSlider>

class CVolumeSliderDialog : public QWidget
{
    Q_OBJECT
public:
    explicit CVolumeSliderDialog(QWidget *parent = nullptr);
    ~CVolumeSliderDialog();

    void setValue(int value);  // 设置音量

signals:
    void signalVolumeChanged(int volume);  // 音量改变时发出信号

private:
    void initUI();  // 初始化UI
    void onVolumeChanged();  // 音量改变时的处理函数

protected:
    void paintEvent(QPaintEvent *event) override;  // 重写绘制事件
    bool event(QEvent *e) override;  // 处理其他事件

private:
    QSlider* m_slider;  // 音量滑块

};

#endif // CVOLUMESLIDERDIALOG_H
