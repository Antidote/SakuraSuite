QT       += core gui network

TARGET = Wiiking2Editor
TEMPLATE = app
INCLUDEPATH += \
    include \
    $$PWD/../PluginFramework/include

target.path = ../
INSTALLS += target

LIBS += -L$$PWD/../PluginFramework -lPluginFramework
SOURCES += \
    $$PWD/src/MainWindow.cpp \
    $$PWD/src/PluginsDialog.cpp \
    $$PWD/src/PluginsManager.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/AboutDialog.cpp

HEADERS += \
    $$PWD/include/Constants.hpp \
    $$PWD/include/MainWindow.hpp \
    $$PWD/include/PluginsDialog.hpp \
    $$PWD/include/PluginsManager.hpp \
    $$PWD/include/AboutDialog.hpp

FORMS += \
    $$PWD/ui/MainWindow.ui \
    $$PWD/ui/PluginsDialog.ui \
    $$PWD/ui/AboutDialog.ui

RESOURCES += \
    $$PWD/resources/resources.qrc
*-64 {
    CONFIG += WIIKING2_PLATFORM_X64
}

message($$QMAKE_HOST.arch)
