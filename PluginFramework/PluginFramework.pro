#-------------------------------------------------
#
# Project created by QtCreator 2013-09-06T08:30:53
#
#-------------------------------------------------

QT       += core gui

TARGET = PluginFramework
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    $$PWD/src/GameFile.cpp

INCLUDEPATH += include
HEADERS += \
    $$PWD/include/GameFile.hpp \
    $$PWD/include/PluginInterface.hpp
