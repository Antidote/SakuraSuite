// This file is part of WiiKing2 Editor.
//
// WiiKing2 Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Wiiking2 Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WiiKing2 Editor.  If not, see <http://www.gnu.org/licenses/>

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
      m_pluginsDialog(new PluginsDialog(parent, this)),
      m_mainWindow(parent)
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

PluginInterface* PluginsManager::plugin(const QString& name)
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
//    return false;
    PluginInterface* plugin = this->plugin(name);

    if (!plugin)
        return false;

    QString pluginPath = plugin->path();

    m_mainWindow->closeFilesFromLoader(plugin);
    QPluginLoader* loader = m_pluginLoaders.take(pluginPath);

    m_plugins.remove(pluginPath);

    if (!loader)
        return false;


    if(loader->unload())
    {

        delete loader;
        loader = NULL;
        loader = new QPluginLoader(pluginPath);

        m_pluginLoaders[pluginPath] = loader;

        if (loader->load())
        {
            QObject* object = loader->instance();
            PluginInterface* newPlugin = qobject_cast<PluginInterface *>(object);
            if (newPlugin)
            {
                newPlugin->setPath(pluginPath);
                QSettings settings;
                settings.beginGroup(newPlugin->name());
                newPlugin->setEnabled(settings.value("enabled", true).toBool());
                m_plugins[pluginPath] = newPlugin;
                return true;
            }
            else
            {
                qDebug() << loader->errorString();
                return false;
            }
        }
        else
        {
            qDebug() << loader->errorString();
        }
    }

    m_pluginLoaders[pluginPath] = loader;
    m_plugins[pluginPath] = plugin;
    qDebug() << loader->errorString();

    return false;
}

QMap<QString, PluginInterface*> PluginsManager::plugins()
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
        QPluginLoader* pluginLoader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *object = pluginLoader->instance();

        if (object)
        {
            PluginInterface* plugin = qobject_cast<PluginInterface *>(object);
            if (plugin)
            {
                m_plugins[pluginsDir.absoluteFilePath(fileName)] = plugin;
                connect(plugin->object(), SIGNAL(enabledChanged()), this, SLOT(onEnabledChanged()));
                plugin->object()->setParent(this->parent());
                QSettings settings;
                settings.beginGroup(plugin->name());
                plugin->setPath(pluginsDir.absoluteFilePath(fileName));
                plugin->setEnabled(settings.value("enabled", true).toBool());
                m_pluginLoaders[pluginsDir.absoluteFilePath(fileName)] = pluginLoader;
            }
            else
            {
                qDebug() << "Failed to load plugin" << fileName;
                qDebug() << pluginLoader->errorString();
                delete pluginLoader;
                pluginLoader = NULL;
            }
        }
        else
        {
            qDebug() << "Failed to load plugin" << fileName;
            qDebug() << pluginLoader->errorString();
            delete pluginLoader;
            pluginLoader = NULL;
            delete pluginLoader;
            pluginLoader = NULL;
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
