cmake_minimum_required(VERSION 3.0)
project(SakuraSuite)
if(NOT MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Gui Xml OpenGL Widgets)

add_definitions(-D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS)

add_subdirectory(PluginFramework)
add_subdirectory(Updater)

set(CMAKE_INCLUDE_CURRENT_DIR True)
set(CMAKE_AUTOMOC True)

qt5_wrap_ui(ui_out
    Main/ui/MainWindow.ui
    Main/ui/PluginsDialog.ui
    Main/ui/PreferencesDialog.ui
    Main/ui/ApplicationLog.ui
    Main/ui/AboutDialog.ui)

qt5_add_resources(rc_out Main/resources/resources.qrc)

include_directories(Main/include PluginFramework/include Updater/include)
add_executable(sakurasuite
    Main/src/main.cpp
    Main/src/MainWindow.cpp Main/include/MainWindow.hpp
    Main/src/PreferencesDialog.cpp Main/include/PreferencesDialog.hpp
    Main/src/OutputStreamMonitor.cpp Main/include/OutputStreamMonitor.hpp
    Main/src/ApplicationLog.cpp Main/include/ApplicationLog.hpp
    Main/src/AboutDialog.cpp Main/include/AboutDialog.hpp
    Main/src/PluginsDialog.cpp Main/include/PluginsDialog.hpp
    Main/src/PluginsManager.cpp Main/include/PluginsManager.hpp
    Main/src/WiiKeyManager.cpp Main/include/WiiKeyManager.hpp
    ${ui_out}
    ${rc_out}
)

target_link_libraries(sakurasuite
    PluginFramework
    Updater

    ${Qt5Core_LIBRARIES}
    ${Qt5Gui_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Xml_LIBRARIES}
)
    