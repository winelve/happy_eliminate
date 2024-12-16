
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "./code/windows/mainwindow.h"

int main(int argc, char *argv[])
{
    // qDebug() << "Hello happy_elimitate" ;
    // // return 0;
    // QApplication a(argc, argv);
    // MainWindow m;
    // m.show();
    // return a.exec();
    QGuiApplication app(argc, argv);

    // 创建 QML 引擎
    QQmlApplicationEngine engine;

    // 加载 QML 文件
    engine.load(QUrl(QStringLiteral("qrc:/lodginwindow.qml")));

    // 检查是否加载成功
    if (engine.rootObjects().isEmpty()) {
        return -1;  // 如果没有加载根对象，返回错误
    }

    return app.exec();
}
