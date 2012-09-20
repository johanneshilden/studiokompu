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
    compwidget.cpp \
    commands/replacenodecommand.cpp \
    commands/insertnodecommand.cpp \
    commands/setprojectioncoordcommand.cpp

HEADERS  += mainwindow.h \
    compnodeitem.h \
    lib/comp.h \
    compgraphicsscene.h \
    compwidget.h \
    commands/replacenodecommand.h \
    commands/insertnodecommand.h \
    commands/setprojectioncoordcommand.h

#QMAKE_CXXFLAGS += -std=c++0x
