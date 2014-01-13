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

#ifndef OOTSAVEPLUGIN_HPP
#define OOTSAVEPLUGIN_HPP

#include <QObject>
#include "ootsaveplugin_global.hpp"
#include <QIcon>
#include "PluginInterface.hpp"

class QAction;

class OOTSAVEPLUGINSHARED_EXPORT OoTSavePlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.wiiking2.ootplugin" FILE "ootsaveplugin.json")
#endif
public:
    OoTSavePlugin();
    ~OoTSavePlugin();

    void initialize(MainWindowBase *mainWindow);
    QString filter()      const;
    QString extension()   const;
    QString name()        const;
    QString author()      const;
    QString version()     const;
    QString website()     const;
    void setPath(const QString &path);
    QString path()        const;
    QString license()     const;
    QString description() const;
    bool enabled() const;
    void setEnabled(const bool enable);

    DocumentBase* loadFile(const QString& file) const;
    bool canLoad(const QString& filename);

    PluginSettingsDialog* settingsDialog();
    Updater* updater();
    QObject* object();

    QIcon icon() const;
    bool hasUpdater() const;
    void doUpdate();
    MainWindowBase* mainWindow() const;
signals:
    void newDocument(DocumentBase *);
    void enabledChanged();
public slots:

private:
    QAction* m_actionNewDocument;
    bool m_enabled;
    QString m_path;
    QIcon m_icon;
    MainWindowBase* m_mainWindow;
};

#endif // OOTSAVEPLUGIN_HPP
