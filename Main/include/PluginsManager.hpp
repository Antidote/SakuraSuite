#ifndef PLUGINSMANAGER_HPP
#define PLUGINSMANAGER_HPP

#include <QObject>
#include <QMap>
class PluginsDialog;
class PluginInterface;
class QPluginLoader;
class MainWindow;

class PluginsManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginsManager(MainWindow* parent);
    ~PluginsManager();

    void dialog();

    PluginInterface* plugin(const QString& name);
    QMap<QString, PluginInterface*> plugins();

    PluginInterface* preferredPlugin(const QString& file);
    bool reloadByName(const QString& name);
signals:

public slots:
    void unloadPlugins();
    void loadPlugins();
    void onEnabledChanged();

private:
    PluginsDialog*  m_pluginsDialog;

    MainWindow*             m_mainWindow;
    QMap<QString, QPluginLoader*> m_pluginLoaders;
    QMap<QString, PluginInterface*> m_plugins;
};

#endif // PLUGINSMANAGER_HPP
