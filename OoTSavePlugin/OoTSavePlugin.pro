#-------------------------------------------------
#
# Project created by QtCreator 2013-09-06T02:05:41
#
#-------------------------------------------------

QT       += core gui

TARGET = OoTSavePlugin
TEMPLATE = lib
CONFIG += plugin

target.path = ../plugins

INSTALLS += target

DEFINES += OOTSAVEPLUGIN_LIBRARY

SOURCES += \
    $$PWD/OoTSavePlugin.cpp

INCLUDEPATH += \
    $$PWD/../PluginFramework/include

LIBS += -L../PluginFramework -lPluginFramework

HEADERS +=\
    $$PWD/OoTSavePlugin.hpp \
    $$PWD/ootsaveplugin_global.hpp

RESOURCES += \
    $$PWD/resources/resources.qrc

