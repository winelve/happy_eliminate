#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPixmap>

#include "board.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;
    QSize GetBoardSize() { return QSize(width_*cell_size_,height_*cell_size_); }

private:
    Board *board_;
    QPixmap board_background_;
    int width_; int height_;
    int cell_size_;
    int padding_;   // 边距
};

#endif // BOARDWIDGET_H
