#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "code/core/boardwidget.h"
#include <QWidget>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private:
    Ui::GameWidget *ui;
    BoardWidget board_widget_;


};

#endif // GAMEWIDGET_H
