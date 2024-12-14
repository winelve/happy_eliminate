#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QMainWindow>

namespace Ui {
class FrameLessWindow;
}

class FrameLessWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FrameLessWindow(QWidget *parent = nullptr);
    ~FrameLessWindow();

private:
    Ui::FrameLessWindow *ui;
};

#endif // FRAMELESSWINDOW_H
