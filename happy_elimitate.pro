QT       += core gui
QT       += multimedia
QT += quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    code/core/Animation/animationcomponent.cpp \
    code/audio/audioplayer.cpp \
    code/core/Animation/rendermanager.cpp \
    code/core/Animation/resourcemanager.cpp \
    code/core/board.cpp \
    code/core/boardmanager.cpp \
    code/core/boardwidget.cpp \
    code/windows/aboutwindow.cpp \
    code/windows/audiocontrol.cpp \
    code/windows/framelesswindow.cpp \
    code/core/dataresource.cpp \
    code/core/entity/coleffect.cpp \
    code/core/entity/eliminateeffect.cpp \
    code/core/entity/magiceffect.cpp \
    code/core/entity/roweffect.cpp \
    code/core/entity/wordeffect.cpp \
    code/core/entity/wordeffect2.cpp \
    code/core/posmanager.cpp \
    code/core/utils/constants.cpp \
    code/core/entity/cube.cpp \
    code/core/entity/entity.cpp \
    code/core/gamelogic.cpp \
    code/core/gamestate/boardstates.cpp \
    code/core/gamestate/statemachine.cpp \
    code/core/gamestate/statenode.cpp \
    code/core/utils/messagetips.cpp \
    code/core/utils/utils.cpp \
    code/core/utils/vector2.cpp \
    code/database/database.cpp \
    code/test/audio_test/testaudiomain.cpp \
    code/windows/gamewidget.cpp \
    code/windows/mainwindow.cpp \
    code/windows/modemanager.cpp \
    code/windows/modeselectionwindow.cpp \
    code/windows/rankwindow.cpp \
    code/windows/settingwindow.cpp \
    code/windows/smallmenuwindow.cpp \
    code/windows/usermanager.cpp \
    main.cpp

HEADERS += \
    code/core/Animation/animationcomponent.h \
    code/audio/audioplayer.h \
    code/core/Animation/rendermanager.h \
    code/core/Animation/resourcemanager.h \
    code/core/board.h \
    code/core/boardmanager.h \
    code/core/boardwidget.h \
    code/windows/aboutwindow.h \
    code/windows/audiocontrol.h \
    code/windows/framelesswindow.h \
    code/core/dataresource.h \
    code/core/entity/coleffect.h \
    code/core/entity/eliminateeffect.h \
    code/core/entity/magiceffect.h \
    code/core/entity/roweffect.h \
    code/core/entity/wordeffect.h \
    code/core/entity/wordeffect2.h \
    code/core/posmanager.h \
    code/core/utils/constants.h \
    code/core/entity/cube.h \
    code/core/entity/entity.h \
    code/core/gamelogic.h \
    code/core/gamestate/boardstates.h \
    code/core/gamestate/statemachine.h \
    code/core/gamestate/statenode.h \
    code/core/utils/messagetips.h \
    code/core/utils/utils.h \
    code/core/utils/vector2.h \
    code/database/database.h \
    code/windows/gamewidget.h \
    code/windows/mainwindow.h \
    code/windows/modemanager.h \
    code/windows/modeselectionwindow.h \
    code/windows/rankwindow.h \
    code/windows/settingwindow.h \
    code/windows/smallmenuwindow.h \
    code/windows/usermanager.h

FORMS += \
    code/windows/aboutwindow.ui \
    code/windows/framelesswindow.ui \
    code/windows/gamewidget.ui \
    code/windows/mainwindow.ui \
    code/windows/modeselectionwindow.ui \
    code/windows/rankwindow.ui \
    code/windows/settingwindow.ui \
    code/windows/smallmenuwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/resource.qrc
    QT += sql
    QT += core sql

