QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    code/audio/audioplayer.cpp \
    code/core/Animation/atlas.cpp \
    code/core/Animation/frameanimation.cpp \
    code/core/board.cpp \
    code/core/boardwidget.cpp \
    code/core/constants.cpp \
    code/core/cube.cpp \
    code/core/vector2.cpp \
<<<<<<< HEAD
    code/windows/aboutwindow.cpp \
    code/windows/cvolumesliderdialog.cpp \
    code/windows/framelesswindow.cpp \
=======
    code/test/audio_test/testaudiomain.cpp \
>>>>>>> master
    code/windows/gamewidget.cpp \
    code/windows/mainwindow.cpp \
    code/windows/settingwindow.cpp \
    main.cpp

HEADERS += \
    code/audio/audioplayer.h \
    code/core/Animation/atlas.h \
    code/core/Animation/frameanimation.h \
    code/core/board.h \
    code/core/boardwidget.h \
    code/core/constants.h \
    code/core/cube.h \
    code/core/vector2.h \
<<<<<<< HEAD
    code/windows/aboutwindow.h \
    code/windows/cvolumesliderdialog.h \
    code/windows/framelesswindow.h \
    code/windows/gamewidget.h \
    code/windows/mainwindow.h \
    code/windows/settingwindow.h
=======
    code/windows/gamewidget.h \
>>>>>>> master

FORMS += \
    code/windows/aboutwindow.ui \
    code/windows/framelesswindow.ui \
    code/windows/gamewidget.ui \
    code/windows/mainwindow.ui \
    code/windows/settingwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/resource.qrc
