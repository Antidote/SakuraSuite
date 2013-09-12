#-------------------------------------------------
#
# Project created by Creator 2013-09-06T02:05:41
#
#-------------------------------------------------

QT    += core gui

TARGET = OoTSavePlugin
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../plugins

TRANSLATIONS += \
    lang/en_ES.ts

DEFINES += OOTSAVEPLUGIN_LIBRARY

SOURCES += \
    OoTSavePlugin.cpp

INCLUDEPATH += \
    ../PluginFramework/include

LIBS += -L../PluginFramework -lpluginframework

HEADERS +=\
    OoTSavePlugin.hpp \
    ootsaveplugin_global.hpp

RESOURCES += \
    resources/resources.qrc

