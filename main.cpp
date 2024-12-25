#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "./code/windows/mainwindow.h"
#include "./code/database/database.h"
#include "./code/windows/usermanager.h"

//#include "code/windows/smallmenuwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 连接QML
    MainWindow mainWindow;
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/loginwidget.qml")));

    // 检查是否加载成功
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    // 在 QML 中设置信号槽连接
    engine.rootContext()->setContextProperty("mainWindow", &mainWindow);

    UserManager um;
    engine.rootContext()->setContextProperty("um", &um);

    DataBase *db = new DataBase();
    db->BuildDatabase();
    engine.rootContext()->setContextProperty("db", db);

    // SmallMenuWindow mw;
    // mw.show();

    // 启动应用
    return a.exec();
}
