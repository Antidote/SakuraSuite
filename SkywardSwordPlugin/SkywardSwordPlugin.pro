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
    src/SkywardSwordPlugin.cpp \
    src/SkywardSwordEditorForm.cpp \
    src/SkywardSwordGameFile.cpp \
    src/SettingsManager.cpp \
    src/SettingsDialog.cpp

INCLUDEPATH += \
    include \
    ../PluginFramework/include \
    ../libzelda/include

LIBS += \
    -L../PluginFramework -lPluginFramework

CONFIG(release, release|debug){
    warning("building in release mode")
    LIBS += -L../libzelda/lib -lzelda
}

CONFIG(debug, debug|release){
    LIBS += -L../libzelda/lib -lzelda-d
}


HEADERS +=\
    ../PluginFramework/include/PluginInterface.hpp \
    ../PluginFramework/include/GameFile.hpp \
    ../PluginFramework/include/EditorForm.hpp \
    include/SkywardSwordPlugin.hpp \
    include/skywardswordplugin_global.hpp \
    include/SkywardSwordEditorForm.hpp \
    include/SkywardSwordGameFile.hpp \
    include/SettingsManager.hpp \
    include/SettingsDialog.hpp

RESOURCES += \
    resources/resources.qrc

FORMS += \
    ui/SkywardSwordEditorForm.ui \
    ui/SettingsDialog.ui

