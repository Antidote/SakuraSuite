#-------------------------------------------------
#
# Project created by QtCreator 2013-09-06T02:05:41
#
#-------------------------------------------------

QT       += core gui

TARGET = SkywardSwordPlugin
TEMPLATE = lib
CONFIG += plugin

target.path = ../plugins

INSTALLS += target

DEFINES += SKYWARDSWORDPLUGGIN_LIBRARY

SOURCES += \
    SkywardSwordPlugin.cpp

INCLUDEPATH += \
    ../PluginFramework/include

LIBS += -L../PluginFramework -lPluginFramework

HEADERS +=\
    ../Main/include/PluginInterface.hpp \
    ../Main/include/GameFile.hpp \
    SkywardSwordPlugin.hpp \
    skywardswordplugin_global.hpp

