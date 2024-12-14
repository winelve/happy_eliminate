#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framelesswindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public FrameLessWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAbout_clicked();

    void on_btnQuit_clicked();

    void on_btnSetting_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
