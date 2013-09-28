TEMPLATE = subdirs
CONFIG += ordered

wiiking2.depends += PluginFramework \
        Updater
SkywardSwordPlugin.depends += libzelda \
        PluginFramework \
        Updater

SUBDIRS += \
    libzelda \
    Updater \
    PluginFramework \
    Main \
    OoTSavePlugin \
    SkywardSwordPlugin 

