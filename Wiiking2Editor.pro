TEMPLATE = subdirs
CONFIG += ordered

wiiking2.depends += PluginFramework \
        Updater
SkywardSwordPlugin.depends += libzelda

SUBDIRS += \
    libzelda \
    PluginFramework \
    Main \
    OoTSavePlugin \
    SkywardSwordPlugin \
    Updater

