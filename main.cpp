
#include <QApplication>


#include "code/core/board.h"
#include "code/windows/gamewidget.h"
#include "code/core/Animation/resourcemanager.h"
#include "code/audio/audioplayer.h"


using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "Hello happy_elimitate" ;
    GameWidget *widget = new GameWidget();
    widget->show();
    ResourceManager::Instance();
    AudioPlayer::getInstance();


    return a.exec();
}
