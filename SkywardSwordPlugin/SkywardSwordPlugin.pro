#-------------------------------------------------
#
# Project created by Creator 2013-09-06T02:05:41
#
#-------------------------------------------------

QT    += core gui

TARGET = SkywardSwordPlugin
TEMPLATE = lib
CONFIG += plugin
TRANSLATIONS += \
    lang/en_ES.ts

DESTDIR = ../plugins

DEFINES += SKYWARDSWORDPLUGGIN_LIBRARY

SOURCES += \
    src/SkywardSwordPlugin.cpp \
    src/SkywardSwordEditorForm.cpp \
    src/SkywardSwordGameFile.cpp \
    src/SettingsManager.cpp \
    src/SettingsDialog.cpp \
    src/PlaytimeWidget.cpp \
    src/Common.cpp

INCLUDEPATH += \
    include \
    ../PluginFramework/include \
    ../libzelda/include

LIBS += \
    -L../PluginFramework -lPluginFramework

CONFIG(release, release|debug){
    LIBS += -L../libzelda/lib -lzelda
}

CONFIG(debug, debug|release){
    LIBS += -L../libzelda/lib -lzelda-d
}

HEADERS +=\
    include/SkywardSwordPlugin.hpp \
    include/skywardswordplugin_global.hpp \
    include/SkywardSwordEditorForm.hpp \
    include/SkywardSwordGameFile.hpp \
    include/SettingsManager.hpp \
    include/SettingsDialog.hpp \
    include/PlaytimeWidget.hpp \
    include/Common.hpp

RESOURCES += \
    resources/resources.qrc

FORMS += \
    ui/SkywardSwordEditorForm.ui \
    ui/SettingsDialog.ui \
    ui/PlaytimeWidget.ui

