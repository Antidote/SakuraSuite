TEMPLATE = subdirs
CONFIG += ordered

Main.depends += PluginFramework \
        Updater
SkywardSwordPlugin.depends += libzelda \
        PluginFramework \
        Updater

SUBDIRS += \
    Updater \
    PluginFramework \
    Main

TRANSLATIONS += \
    en_US.ts
