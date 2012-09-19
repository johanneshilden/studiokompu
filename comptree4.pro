#-------------------------------------------------
#
# Project created by QtCreator 2012-09-18T22:12:57
#
#-------------------------------------------------

QT       += core gui

TARGET = comptree4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lib/comp.c \
    compnodeitem.cpp \
    compgraphicsscene.cpp \
    compwidget.cpp

HEADERS  += mainwindow.h \
    compnodeitem.h \
    lib/comp.h \
    compgraphicsscene.h \
    compwidget.h

#QMAKE_CXXFLAGS += -std=c++0x
