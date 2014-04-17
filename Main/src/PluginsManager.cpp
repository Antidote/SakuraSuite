// This file is part of Sakura Suite.
//
// Sakura Suite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Sakura Suite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura Suite.  If not, see <http://www.gnu.org/licenses/>

#include "PluginsManager.hpp"
#include "MainWindow.hpp"
#include "PluginsDialog.hpp"
#include "PluginInterface.hpp"
#include <QApplication>
#include <QPluginLoader>
#include <QDir>

PluginsManager::PluginsManager(MainWindow* parent)
    : QObject(parent),
      m_pluginsDialog(new PluginsDialog(parent, this)),
      m_mainWindow(parent)
{
}

PluginsManager::~PluginsManager()
{
    foreach (QPluginLoader* loader, m_pluginLoaders.values())
    {
        if (loader->isLoaded())
            loader->unload();

        delete loader;
        loader = NULL;
    }

    m_pluginLoaders.clear();
    m_plugins.clear();
    delete m_pluginsDialog;
}

void PluginsManager::dialog()
{
    m_pluginsDialog->exec();
}

PluginInterface* PluginsManager::plugin(const QString& name)
{
    foreach(PluginInterface* plugin, m_plugins)
    {
        if (!QString::compare(plugin->name(), name, Qt::CaseInsensitive))
            return plugin;
    }

    return NULL;
}

bool PluginsManager::reloadByName(const QString& name)
{
    PluginInterface* plugin = this->plugin(name);

    if (!plugin)
        return false;

    QString pluginPath = plugin->path();

    m_mainWindow->closeFilesFromLoader(plugin);
    QPluginLoader* loader = m_pluginLoaders.take(name.toLower());

    m_plugins.removeAll(plugin);

    if (!loader)
        return false;

    if(loader->unload())
    {
        delete loader;
        loader = NULL;
        QThread::msleep(1000);
        loader = new QPluginLoader(pluginPath);

        m_pluginLoaders[name.toLower()] = loader;

        if (loader->load())
        {
            QObject* object = loader->instance();
            PluginInterface* newPlugin = qobject_cast<PluginInterface *>(object);
            if (newPlugin)
            {
                newPlugin->setPath(pluginPath);
                newPlugin->object()->setParent(parent());
                m_mainWindow->addFileFilter(newPlugin->filter());
                QSettings settings;
                settings.beginGroup(newPlugin->name());
                newPlugin->setEnabled(settings.value("enabled", true).toBool());
                qDebug() << newPlugin->author();
                newPlugin->initialize(m_mainWindow);
                connect(newPlugin->object(), SIGNAL(newDocument(DocumentBase*)), m_mainWindow, SLOT(onNewDocument(DocumentBase*)));
                m_plugins.append(newPlugin);
                return true;
            }
            else
            {
                loader->unload();
                m_mainWindow->error(tr("Error loading %1: %2")
                                    .arg(pluginPath)
                                    .arg(loader->errorString()));
                return false;
            }
        }
        else
        {
            m_mainWindow->error(tr("Error loading %1: %2")
                                .arg(pluginPath)
                                .arg(loader->errorString()));
            return false;
        }
    }

    m_pluginLoaders[name.toLower()] = loader;
    m_plugins.append(plugin);
    m_mainWindow->error(tr("Error loading %1: %2")
                        .arg(pluginPath)
                        .arg(loader->errorString()));

    return false;
}

QList<PluginInterface*> PluginsManager::plugins()
{
    return m_plugins;
}

PluginInterface* PluginsManager::preferredPlugin(const QString& file)
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

    // If we are unable to cd into the plugins directory, we should try the applications directory
    // in the users home path
    if (!pluginsDir.cd("plugins"))
    {
        pluginsDir = QDir(Constants::SAKURASUITE_HOME_PATH);
        if (!pluginsDir.cd("plugins"))
        {
            m_mainWindow->error(tr("Unable to acquire plugin directory"));
            return;
        }
        qDebug() << pluginsDir.absolutePath();

    }

    foreach (QString fileName, pluginsDir.entryList(QStringList() << Constants::SAKURASUITE_PLUGIN_EXTENSION, QDir::Files))
    {
        QPluginLoader* loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
        if (loader->instance())
        {
            PluginInterface* plugin = qobject_cast<PluginInterface *>(loader->instance());
            if (plugin)
            {
                if (!this->plugin(plugin->name()))
                {
                    m_plugins.append(plugin);
                    connect(plugin->object(), SIGNAL(enabledChanged()), this, SLOT(onEnabledChanged()));
                    plugin->object()->setParent(this->parent());
                    m_mainWindow->addFileFilter(plugin->filter());
                    QSettings settings;
                    settings.beginGroup(plugin->name());
                    plugin->setPath(pluginsDir.absoluteFilePath(fileName));
                    plugin->setEnabled(settings.value("enabled", true).toBool());
                    plugin->initialize(m_mainWindow);
                    connect(plugin->object(), SIGNAL(newDocument(DocumentBase*)), m_mainWindow, SLOT(onNewDocument(DocumentBase*)));
                    m_pluginLoaders[plugin->name().toLower()] = loader;
                }
                else
                {
                    m_mainWindow->error(tr("Error loading %1: %2")
                                        .arg(fileName)
                                        .arg(loader->errorString()));
                    loader->unload();
                    delete loader;
                }
            }
            else
            {
                loader->unload();
                m_mainWindow->error(tr("Error loading %1: %2")
                                    .arg(fileName)
                                    .arg(loader->errorString()));
                delete loader;
                loader = NULL;
            }
        }
        else
        {
            m_mainWindow->error(tr("Error loading %1: %2")
                                .arg(fileName)
                                .arg(loader->errorString()));
            delete loader;
            loader = NULL;
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
            parent->removeFileFilter(plugin->filter());
        else
            parent->addFileFilter(plugin->filter());
    }
}
