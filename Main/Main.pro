QT       += core gui

TARGET = Wiiking2Editor
TEMPLATE = app
INCLUDEPATH += \
    include \
    ../PluginFramework/include

target.path = ../
INSTALLS += target

LIBS += -L../PluginFramework -lPluginFramework
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
