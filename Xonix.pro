#-------------------------------------------------
#
# Project created by QtCreator 2017-02-03T13:23:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Xonix
TEMPLATE = app


SOURCES += main.cpp \
    xonixmodel.cpp \
    xonixview.cpp \
    xonixcontroller.cpp \
    screen.cpp \
    ball.cpp \
    cube.cpp \
    xonix.cpp \
    enemy.cpp

HEADERS  += \
    xonixmodel.h \
    xonixview.h \
    xonixcontroller.h \
    screen.h \
    ball.h \
    cube.h \
    xonix.h \
    enemy.h

CONFIG += c++14

FORMS    +=

QMAKE_CXXFLAGS += -std=gnu++14
