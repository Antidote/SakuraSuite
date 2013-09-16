TEMPLATE = subdirs
CONFIG += ordered

wiiking2.depends += PluginFramework \
        Updater
SkywardSwordPlugin.depends += libzelda \
        Updater

SUBDIRS += \
    libzelda \
    Updater \
    PluginFramework \
    Main \
    OoTSavePlugin \
    SkywardSwordPlugin

