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
    QList<PluginInterface*> plugins();

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
    QList<PluginInterface*> m_plugins;
};

#endif // PLUGINSMANAGER_HPP
