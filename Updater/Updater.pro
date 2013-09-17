#-------------------------------------------------
#
# Project created by QtCreator 2013-09-14T13:57:34
#
#-------------------------------------------------

QT       += network gui

TARGET = updater
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD

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
INCLUDEPATH += include
SOURCES += src/Updater.cpp

HEADERS += include/Updater.hpp \
    include/Constants.hpp
