QT       += core gui network

TARGET = Wiiking2Editor
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

DESTDIR = ../
LIBS += -L../PluginFramework -lPluginFramework

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
