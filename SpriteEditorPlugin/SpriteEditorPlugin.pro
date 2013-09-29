#-------------------------------------------------
#
# Project created by QtCreator 2013-09-28T18:22:25
#
#-------------------------------------------------

QT       += core gui xml opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spriteeditorplugin
TEMPLATE = lib
CONFIG += plugin

DEFINES += SPRITEEDITORPLUGIN_LIBRARY

INCLUDEPATH += \
    include \
    ../PluginFramework/include \
    ../libzelda/include \
    ../Updater/include

DESTDIR = ../plugins
UI_DIR = ui

QMAKE_CXXFLAGS = -std=c++0x

LIBS += \
    -L../PluginFramework -lpluginframework \
    -L../Updater -lupdater

CONFIG(release, release|debug){
    LIBS += -L../libzelda/lib -lzelda
    DEFINES -= SS_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
    MOC_DIR = moc/release
}


CONFIG(debug, debug|release){
    LIBS += -L../libzelda/lib -lzelda-d
    DEFINES += SS_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
    MOC_DIR = moc/debug
}


SOURCES += src/SpriteEditorPlugin.cpp \
    src/SpriteDocument.cpp

HEADERS += include/SpriteEditorPlugin.hpp\
        include/spriteeditorplugin_global.hpp \
    include/SpriteDocument.hpp

OTHER_FILES += \
    spriteeditor.json

