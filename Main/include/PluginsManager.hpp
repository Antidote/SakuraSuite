#ifndef PLUGINSMANAGER_HPP
#define PLUGINSMANAGER_HPP

#include <QObject>

class PluginsDialog;
class PluginInterface;
class MainWindow;

class PluginsManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginsManager(MainWindow* parent);

    void dialog();

    PluginInterface* pluginById(const int id);
    PluginInterface* pluginByName(const QString& name);
    QList<PluginInterface*> plugins();

    PluginInterface* preferredPlugin(const QString& file);
signals:

public slots:
    void unloadPlugins();
    void loadPlugins();
    void onEnabledChanged();

private:
    PluginsDialog*  m_pluginsDialog;

    QList<PluginInterface*> m_plugins;
};

#endif // PLUGINSMANAGER_HPP
