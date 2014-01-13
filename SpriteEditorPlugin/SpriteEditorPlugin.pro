#-------------------------------------------------
#
# Project created by QtCreator 2013-09-28T18:22:25
#
#-------------------------------------------------

QT       += core gui xml
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

DESTDIR = $$OUT_PWD/../plugins
UI_DIR = ui

QMAKE_CXXFLAGS = -std=c++0x

LIBS += \
    -L$$OUT_PWD/../PluginFramework -lpluginframework \
    -L$$OUT_PWD/../Updater -lupdater

CONFIG(release, release|debug){
    LIBS += -L$$OUT_PWD/../libzelda/lib -lzelda
    DEFINES -= SS_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
    MOC_DIR = moc/release
}


CONFIG(debug, debug|release){
    LIBS += -L$$OUT_PWD/../libzelda/lib -lzelda-d
    DEFINES += SS_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
    MOC_DIR = moc/debug
}


SOURCES += \
    src/SpriteEditorPlugin.cpp \
    src/SpriteDocument.cpp \
    src/SpriteInfo.cpp \
    src/SpriteEditorFrame.cpp \
    src/SpriteTree.cpp \
    src/SpriteCanvas.cpp \
    src/SSprite.cpp \
    src/SSpriteFile.cpp \
    src/SSpriteFileReader.cpp \
    src/SSpriteFileWriter.cpp \
    src/SSpriteFrame.cpp \
    src/SSpritePart.cpp

HEADERS += \
    include/SpriteEditorPlugin.hpp\
    include/spriteeditorplugin_global.hpp \
    include/SpriteDocument.hpp \
    include/SpriteInfo.hpp \
    include/SpritePart.hpp \
    include/SpriteEditorFrame.hpp \
    include/SpriteTree.hpp \
    include/SpriteCanvas.hpp \
    include/SSprite.hpp \
    include/SSpriteFile.hpp \
    include/SSpriteFileReader.hpp \
    include/SSpriteFileWriter.hpp \
    include/SSpriteFrame.hpp \
    include/SSpritePart.hpp

OTHER_FILES += \
    spriteeditor.json

FORMS += \
    ui/SpriteInfo.ui \
    ui/SpriteEditorFrame.ui \
    ui/SpriteTree.ui

