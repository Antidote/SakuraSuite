#-------------------------------------------------
#
# Project created by Creator 2013-09-06T08:30:53
#
#-------------------------------------------------

QT    += core gui

TARGET = pluginframework
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD

SOURCES += \
    src/GameFile.cpp

INCLUDEPATH += include
HEADERS += \
    include/GameFile.hpp \
    include/PluginInterface.hpp
