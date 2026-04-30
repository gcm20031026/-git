#-------------------------------------------------
#
# Project created by QtCreator 2025-05-21T10:58:37
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Security_surveillance_system
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        Control/formatoption.cpp \
        Control/infowidget.cpp \
        Control/jdtmypushbutton.cpp \
        Control/mycamera.cpp \
        Control/pathwitheditbutton.cpp \
        Control/versoninfowidget.cpp \
        View/alarmwin.cpp \
        View/configurationwin.cpp \
        View/loginwindow.cpp \
        View/plyaviewwidget.cpp \
        View/replaywin.cpp \
        View/retrievalwin.cpp \
        buildTarVideo/cdecodetorgb.cpp \
        buildTarVideo/cdecodetoyuv.cpp \
        buildTarVideo/cencodetoh264.cpp \
        buildTarVideo/ctrancetodest.cpp \
        buildTarVideo/opencvdetect.cpp \
    data/qmysqlite.cpp \
    data/userstorage.cpp \
        main.cpp \
        playThread/audiodecodethread.cpp \
        playThread/decodethread.cpp \
        playThread/ffmpegwriterdest.cpp \
        view/mainwindow.cpp \
    playThread/ffmpegtorgbkun.cpp

HEADERS += \
        Control/formatoption.h \
        Control/infowidget.h \
        Control/jdtmypushbutton.h \
        Control/mycamera.h \
        Control/pathwitheditbutton.h \
        Control/versoninfowidget.h \
        View/alarmwin.h \
        View/configurationwin.h \
        View/loginwindow.h \
        View/plyaviewwidget.h \
        View/replaywin.h \
        View/retrievalwin.h \
        buildTarVideo/cdecodetorgb.h \
        buildTarVideo/cdecodetoyuv.h \
        buildTarVideo/cencodetoh264.h \
        buildTarVideo/ctrancetodest.h \
        buildTarVideo/opencvdetect.h \
    data/qmysqlite.h \
    data/userstorage.h \
        playThread/audiodecodethread.h \
        playThread/decodethread.h \
        playThread/ffmpegwriterdest.h \
        view/mainwindow.h \
    playThread/ffmpegtorgbkun.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
DESTDIR = bin#未来好打包

# 头文件路径
INCLUDEPATH += D:/biyesheji/Security_surveillance_system/Security_surveillance_system2/OpenCV_X86/include
# 库文件路径
LIBS += D:/biyesheji/Security_surveillance_system/Security_surveillance_system2/OpenCV_X86/x86/mingw/lib/libopencv_*.dll.a
INCLUDEPATH += ./ffmpeglib/include

LIBS+= $$PWD/lib/sqlite3.lib
LIBS += $$PWD/ffmpeglib/lib/avcodec.lib  \
        $$PWD/ffmpeglib/lib/avdevice.lib  \
        $$PWD/ffmpeglib/lib/avfilter.lib  \
        $$PWD/ffmpeglib/lib/avformat.lib  \
        $$PWD/ffmpeglib/lib/avutil.lib  \
        $$PWD/ffmpeglib/lib/swresample.lib  \
        $$PWD/ffmpeglib/lib/swscale.lib

