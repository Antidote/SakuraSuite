#include "PluginsManager.hpp"
#include "MainWindow.hpp"
#include "PluginsDialog.hpp"
#include "PluginInterface.hpp"
#include <QApplication>
#include <QPluginLoader>
#include <QDebug>
#include <QDir>

PluginsManager::PluginsManager(MainWindow* parent)
    : QObject(parent),
      m_pluginsDialog(new PluginsDialog(parent, this))
{
}

PluginsManager::~PluginsManager()
{
    foreach (PluginInterface* plugin, m_plugins)
        delete plugin;
    m_plugins.clear();
}

void PluginsManager::dialog()
{
    m_pluginsDialog->exec();
}

PluginInterface* PluginsManager::pluginById(const int id)
{
    if (id < 0 || id > m_plugins.count())
        return NULL;

    return m_plugins[id];
}

PluginInterface* PluginsManager::pluginByName(const QString& name)
{
    foreach(PluginInterface* plugin, m_plugins)
    {
        if (plugin->name() == name)
            return plugin;
    }

    return NULL;
}

bool PluginsManager::reloadByName(const QString& name)
{
    PluginInterface* plugin = pluginByName(name);

    if (!plugin)
        return false;

    QString pluginPath = plugin->path();

    m_plugins.removeOne(plugin);
    delete plugin;

    QPluginLoader pluginLoader(pluginPath);
    QObject *object = pluginLoader.instance();

    if (object)
    {
        plugin = qobject_cast<PluginInterface *>(object);
        if (plugin)
        {
            m_plugins.append(plugin);
            connect(plugin->object(), SIGNAL(enabledChanged()), this, SLOT(onEnabledChanged()));
            QSettings settings;
            settings.beginGroup(plugin->name());
            plugin->setPath(pluginPath);
            plugin->setEnabled(settings.value("enabled", true).toBool());
            return true;
        }
        else
        {
            qDebug() << "Failed to load plugin" << pluginPath;
            qDebug() << pluginLoader.errorString();
        }
    }
    else
    {
        qDebug() << "Failed to load plugin" << pluginPath;
        qDebug() << pluginLoader.errorString();
    }

    return false;
}

QList<PluginInterface*> PluginsManager::plugins()
{
    return m_plugins;
}

PluginInterface*PluginsManager::preferredPlugin(const QString& file)
{
    foreach (PluginInterface* plugin, m_plugins)
    {
        if (plugin->enabled() && plugin->canLoad(file))
            return plugin;
    }

    return NULL;
}

void PluginsManager::unloadPlugins()
{

}

void PluginsManager::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    qDebug() << pluginsDir.absolutePath();

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        qDebug() << fileName;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *object = pluginLoader.instance();

        if (object)
        {
            PluginInterface* plugin = qobject_cast<PluginInterface *>(object);
            if (plugin)
            {
                m_plugins.append(plugin);
                connect(plugin->object(), SIGNAL(enabledChanged()), this, SLOT(onEnabledChanged()));
                QSettings settings;
                settings.beginGroup(plugin->name());
                plugin->setPath(pluginsDir.absoluteFilePath(fileName));
                plugin->setEnabled(settings.value("enabled", true).toBool());
            }
            else
            {
                qDebug() << "Failed to load plugin" << fileName;
                qDebug() << pluginLoader.errorString();
            }
        }
        else
        {
            qDebug() << "Failed to load plugin" << fileName;
            qDebug() << pluginLoader.errorString();
        }
    }
}

void PluginsManager::onEnabledChanged()
{
    PluginInterface* plugin = qobject_cast<PluginInterface*>(sender());

    if (plugin)
    {
        MainWindow* parent = (MainWindow*)this->parent();
        QSettings settings;
        settings.beginGroup(plugin->name());
        settings.setValue("enabled", plugin->enabled());
        if (!plugin->enabled())
        {

            parent->removeFileFilter(plugin->filter());
        }
        else
        {
            parent->addFileFilter(plugin->filter());
        }
    }
}
