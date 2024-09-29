#-------------------------------------------------
#
# Project created by QtCreator 2020-11-17T18:41:57
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SamanParkKiosk
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    voiceplayerworker.cpp

HEADERS  += mainwidget.h \
    voiceplayerworker.h

FORMS    += mainwidget.ui


win32:CONFIG(release, debug|release): LIBS += -L$$/usr/local/lib/release/ -lwiringPi
else:win32:CONFIG(debug, debug|release): LIBS += -L$$/usr/local/lib/debug/ -lwiringPi
else:unix: LIBS += -L$$/usr/local/lib/ -lwiringPi

INCLUDEPATH += $$/usr/local/include
DEPENDPATH += $$/usr/local/include

INCLUDEPATH += /usr/lib/arm-linux-gnueabihf
INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/local
INCLUDEPATH += /usr/include/c++/4.3


win32:CONFIG(release, debug|release): LIBS += -L$$/usr/lib/release/ -lCSJPOSLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$/usr/lib/debug/ -lCSJPOSLib
else:unix: LIBS += -L$$/usr/lib/ -lCSJPOSLib

INCLUDEPATH += $$usr/include
DEPENDPATH += $$usr/include
