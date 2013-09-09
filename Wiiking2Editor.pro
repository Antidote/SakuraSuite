TEMPLATE = subdirs
CONFIG += ordered

wiiking2.depends += PluginFramework
SkywardSwordPlugin.depends += libzelda

SUBDIRS += \
    libzelda \
    PluginFramework \
    Main \
    OoTSavePlugin \
    SkywardSwordPlugin

