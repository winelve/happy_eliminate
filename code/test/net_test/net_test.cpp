// #include <QApplication>
// #include "code/net/server.h"
// #include "code/net/socketer.h"



// int main(int argc, char *argv[])
// {
//      QApplication a(argc, argv);
//      qDebug() << "Hello happy_elimitate" ;


//      Server server;
//      Socketer socketer;
//      socketer.connectToServer("127.0.0.1", 1234); // 连接到服务器

//      socketer.sendMessage("111_getRanks"); // 发送消息给服务器

//      QObject::connect(&socketer, &Socketer::ranksReceived, [&](const QList<QPair<QString, int>>& tempUserScores) {
//          QList<QPair<QString, int>> userScores=tempUserScores;
//          for (const auto &userScore : userScores) {
//              qDebug() << "用户姓名:" << userScore.first << ", 得分:" << userScore.second;
//          }

//          socketer.sendMessage("111_getRanks successfully");

//      });

//       socketer.addNewScores("ss",10);
//       socketer.addNewScores("ss",10);

//      return a.exec();

// }
