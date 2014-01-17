TEMPLATE = subdirs
CONFIG += ordered

Main.depends += PluginFramework \
        Updater

SUBDIRS += \
    Updater \
    PluginFramework \
    Main

TRANSLATIONS += \
    en_US.ts

OTHER_FILES += \
    .travis.yml
