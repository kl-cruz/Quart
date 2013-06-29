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
    src_hex/xbytearray.cpp \
    src_hex/qhexedit_p.cpp \
    src_hex/qhexedit.cpp \
    src_hex/commands.cpp

HEADERS  += mainwindow.h \
    src_hex/xbytearray.h \
    src_hex/qhexedit_p.h \
    src_hex/qhexedit.h \
    src_hex/commands.h

include(src/qextserialport.pri)

RESOURCES += \
    res.qrc

OTHER_FILES += \
    src_hex/license.txt
