#-------------------------------------------------
#
# Project created by Creator 2013-09-06T02:05:41
#
#-------------------------------------------------

QT    += core gui network

TARGET = skywardswordplugin
TEMPLATE = lib
CONFIG += plugin
TRANSLATIONS += \
    lang/en_ES.ts

DESTDIR = ../plugins
UI_DIR = ui

DEFINES += SKYWARDSWORDPLUGGIN_LIBRARY

SOURCES += \
    src/SkywardSwordPlugin.cpp \
    src/SkywardSwordEditorForm.cpp \
    src/SkywardSwordGameFile.cpp \
    src/SettingsManager.cpp \
    src/SettingsDialog.cpp \
    src/PlaytimeWidget.cpp \
    src/Common.cpp \
    src/TriforceWidget.cpp \
    src/CopyWidget.cpp \
    src/SkywardSwordTabWidget.cpp

INCLUDEPATH += \
    include \
    ../PluginFramework/include \
    ../libzelda/include \
    ../Updater/include

QMAKE_CXXFLAGS = -std=c++0x

LIBS += \
    -L../PluginFramework -lpluginframework \
    -L../Updater -lupdater

CONFIG(release, release|debug){
    LIBS += -L../libzelda/lib -lzelda
    DEFINES -= WK2_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
    MOC_DIR = moc/release
}


CONFIG(debug, debug|release){
    LIBS += -L../libzelda/lib -lzelda-d
    DEFINES += WK2_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
    MOC_DIR = moc/debug
}

HEADERS +=\
    include/SkywardSwordPlugin.hpp \
    include/skywardswordplugin_global.hpp \
    include/SkywardSwordEditorForm.hpp \
    include/SkywardSwordGameFile.hpp \
    include/SettingsManager.hpp \
    include/SettingsDialog.hpp \
    include/PlaytimeWidget.hpp \
    include/Common.hpp \
    include/Constants.hpp \
    include/TriforceWidget.hpp \
    include/CopyWidget.hpp \
    include/SkywardSwordTabWidget.hpp

RESOURCES += \
    resources/resources.qrc
OTHER_FILES += \
    resources/resource.rc

FORMS += \
    ui/SkywardSwordEditorForm.ui \
    ui/SettingsDialog.ui \
    ui/PlaytimeWidget.ui \
    ui/CopyWidget.ui \
    ui/SkywardSwordTabWidget.ui

win32:RC_FILE += resources/resource.rc
