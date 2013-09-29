#-------------------------------------------------
#
# Project created by Creator 2013-09-06T02:05:41
#
#-------------------------------------------------

QT    += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = ootsaveplugin
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../plugins
UI_DIR = ui

CONFIG(release, release|debug) {
    DEFINES -= WK2_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
    MOC_DIR = moc/release
}

CONFIG(debug, debug|release) {
    DEFINES += WK2_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
    MOC_DIR = moc/debug
}

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

OTHER_FILES += \
    ootsaveplugin.json

