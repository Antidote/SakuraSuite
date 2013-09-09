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
    OoTSavePlugin.cpp

INCLUDEPATH += \
    ../PluginFramework/include

LIBS += -L../PluginFramework -lPluginFramework

HEADERS +=\
    ../PluginFramework/include/PluginInterface.hpp \
    ../PluginFramework/include/GameFile.hpp \
    OoTSavePlugin.hpp \
    ootsaveplugin_global.hpp

RESOURCES += \
    resources/resources.qrc

