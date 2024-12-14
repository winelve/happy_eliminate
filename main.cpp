
#include <QApplication>

#include "./code/windows/mainwindow.h"

int main(int argc, char *argv[])
{
    qDebug() << "Hello happy_elimitate" ;
    // return 0;
    QApplication a(argc, argv);
    MainWindow m;
    m.show();
    return a.exec();
}
