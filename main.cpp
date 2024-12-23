
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>


#include "./code/windows/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 连接QML
    MainWindow mainWindow;
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/lodginwindow.qml")));

    // 检查是否加载成功
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    // 在 QML 中设置信号槽连接
    engine.rootContext()->setContextProperty("mainWindow", &mainWindow);
    // 启动应用
    return a.exec();
}
