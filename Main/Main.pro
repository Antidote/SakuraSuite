QT       += core gui network
CONFIG  += embed_manifest_exe

TARGET = wiiking2editor
TEMPLATE = app
INCLUDEPATH += \
    include \
    ../PluginFramework/include

CONFIG(release, release|debug){
    DEFINES -= WK2_DEBUG
}

CONFIG(debug, debug|release){
    DEFINES += WK2_DEBUG
}
win32:RC_FILE = resources/mainicon.rc

DESTDIR = ../
LIBS += -L../PluginFramework -lpluginframework

TRANSLATIONS += \
    lang/en_ES.ts

SOURCES += \
    src/MainWindow.cpp \
    src/PluginsDialog.cpp \
    src/PluginsManager.cpp \
    src/main.cpp \
    src/AboutDialog.cpp

HEADERS += \
    include/Constants.hpp \
    include/MainWindow.hpp \
    include/PluginsDialog.hpp \
    include/PluginsManager.hpp \
    include/AboutDialog.hpp

FORMS += \
    ui/MainWindow.ui \
    ui/PluginsDialog.ui \
    ui/AboutDialog.ui

RESOURCES += \
    resources/resources.qrc

OTHER_FILES += \
    resources/mainicon.rc \
    resources/mainicon.ico
