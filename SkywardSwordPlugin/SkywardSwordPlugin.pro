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
    $$PWD/src/SkywardSwordPlugin.cpp \
    $$PWD/src/SkywardSwordEditorForm.cpp \
    $$PWD/src/SkywardSwordGameFile.cpp \
    $$PWD/src/SettingsManager.cpp \
    $$PWD/src/SettingsDialog.cpp

INCLUDEPATH += \
    include \
    $$PWD/../PluginFramework/include \
    $$PWD/../libzelda/include

LIBS += \
    -L$$PWD/../PluginFramework -lPluginFramework

CONFIG(release, release|debug){
    LIBS += -L$$PWD/../libzelda/lib -lzelda
}

CONFIG(debug, debug|release){
    LIBS += -L$$PWD/../libzelda/lib -lzelda-d
}

message(Project Path: $$PWD)
HEADERS +=\
    $$PWD/../PluginFramework/include/PluginInterface.hpp \
    $$PWD/../PluginFramework/include/GameFile.hpp \
    $$PWD/include/SkywardSwordPlugin.hpp \
    $$PWD/include/skywardswordplugin_global.hpp \
    $$PWD/include/SkywardSwordEditorForm.hpp \
    $$PWD/include/SkywardSwordGameFile.hpp \
    $$PWD/include/SettingsManager.hpp \
    $$PWD/include/SettingsDialog.hpp

RESOURCES += \
    $$PWD/resources/resources.qrc

FORMS += \
    $$PWD/ui/SkywardSwordEditorForm.ui \
    $$PWD/ui/SettingsDialog.ui

