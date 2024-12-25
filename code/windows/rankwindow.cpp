#include "rankwindow.h"
#include "ui_rankwindow.h"
#include "mainwindow.h"
#include "code/database/database.h"
#include "usermanager.h"

#include <QFontDatabase>
#include <QList>
#include <QListWidgetItem>

RankWindow::RankWindow(QWidget *parent)
    : FrameLessWindow(parent)
    , ui(new Ui::RankWindow)
{
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(900,675);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    // 设置字体
    int fontId = QFontDatabase::addApplicationFont(":/font/font.ttf");
    if (fontId == -1) {
        qWarning() << "没有找到字体！";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        ui->label->setFont(font);
        ui->label->setStyleSheet(
            "QLabel { "
            "font-size: 26px; "
            "color: #508330; "
            "}"
            );
    }
    // 数据库连接相关
    DataBase *dataBase = new DataBase();
    dataBase->BuildDatabase();
    QList rank_list = dataBase->fetchUsersByScore();
    addListItems(rank_list);
}

RankWindow::~RankWindow(){delete ui;}

void RankWindow::on_btnReturn_clicked()
{
    audioPlayer->PlaySoundEffect("click.bubble.mp3");
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    delay(150);
    this->close();
}

void RankWindow::addListItems(QList<QPair<QString, int> > rank_list)
{
    int fontId = QFontDatabase::addApplicationFont(":/font/font.ttf");
    if (fontId == -1) {
        qWarning() << "没有找到字体！";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        font.setPointSize(15);

        // 添加数据项
        for (int i = 0; i < rank_list.length(); ++i) {
            QString username = rank_list[i].first;
            int score = rank_list[i].second;
            // 创建一个列表项，显示排名、用户名和得分
            QString displayText = QString("%1                   %2").arg(i + 1).arg(username); // 使用4个空格
            qDebug() << displayText;
            QListWidgetItem *item1 = new QListWidgetItem(displayText);
            QListWidgetItem *item2 = new QListWidgetItem(QString::number(score));
            item1->setSizeHint(QSize(ui->listWidget->width(), 40));  // 每个项目的高度设置为30
            item2->setSizeHint(QSize(ui->listWidget_2->width(), 40));
            item1->setFont(font);
            item2->setFont(font);
            ui->listWidget->addItem(item1);
            ui->listWidget_2->addItem(item2);
        }
    }
}

