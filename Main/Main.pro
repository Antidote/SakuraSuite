QT       += core gui network
CONFIG  += embed_manifest_exe

TARGET = wiiking2editor
TEMPLATE = app
INCLUDEPATH += \
    include \
    ../PluginFramework/include \
    ../Updater/include

UI_DIR = ui

CONFIG(release, release|debug){
    DEFINES -= WK2_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
    MOC_DIR = moc/release
}

CONFIG(debug, debug|release){
    DEFINES += WK2_DEBUG
    # We don't want the objects, or MOC sources
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
    MOC_DIR = moc/debug
}

win32:RC_FILE = resources/mainicon.rc

DESTDIR = ../
LIBS += -L../PluginFramework -lpluginframework \
        -L../Updater -lupdater

TRANSLATIONS += \
    lang/en_ES.ts

SOURCES += \
    src/MainWindow.cpp \
    src/PluginsDialog.cpp \
    src/PluginsManager.cpp \
    src/main.cpp \
    src/AboutDialog.cpp \
    src/PreferencesDialog.cpp

HEADERS += \
    include/Constants.hpp \
    include/MainWindow.hpp \
    include/PluginsDialog.hpp \
    include/PluginsManager.hpp \
    include/AboutDialog.hpp \
    include/PreferencesDialog.hpp

FORMS += \
    ui/MainWindow.ui \
    ui/PluginsDialog.ui \
    ui/AboutDialog.ui \
    ui/PreferencesDialog.ui

RESOURCES += \
    resources/resources.qrc

OTHER_FILES += \
    resources/mainicon.rc \
    resources/mainicon.ico
