TEMPLATE = subdirs
CONFIG += ordered

Main.depends += PluginFramework \
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
    SkywardSwordPlugin \ 
    SpriteEditorPlugin

