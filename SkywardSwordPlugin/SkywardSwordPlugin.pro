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
    SkywardSwordPlugin.cpp \
    SkywardSwordEditorForm.cpp \
    SkywardSwordGameFile.cpp

INCLUDEPATH += \
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
    SkywardSwordPlugin.hpp \
    skywardswordplugin_global.hpp \
    SkywardSwordEditorForm.hpp \
    SkywardSwordGameFile.hpp

RESOURCES += \
    resources/resources.qrc

FORMS += \
    SkywardSwordEditorForm.ui

