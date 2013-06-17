#-------------------------------------------------
#
# Project created by QtCreator 2013-06-11T02:45:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IWSK
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \

include(src/qextserialport.pri)

RESOURCES += \
    res.qrc
