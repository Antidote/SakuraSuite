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

#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <QtCore>
#include <QString>

class Updater;
class DocumentBase;
class WiiKeyManagerBase;
class QDialog;
class MainWindowBase;

class PluginInterface
{
public:
    virtual ~PluginInterface(){}

    virtual void initialize(MainWindowBase* mainWindow)=0;
    virtual QString filter()      const=0;
    virtual QString extension()   const=0;
    virtual QString name() const=0;
    virtual QString author() const=0;
    virtual QString version() const=0;
    virtual QString website() const=0;
    virtual void setPath(const QString& path)=0;
    virtual QString path() const=0;
    virtual QString license() const=0;
    virtual QString description() const=0;
    virtual bool enabled() const=0;
    virtual void setEnabled(const bool enable = true)=0;
    virtual bool canLoad(const QString& file)=0;

    virtual DocumentBase* loadFile(const QString& file) const=0;

    virtual QDialog* settingsDialog()=0;
    virtual Updater* updater()=0;
    virtual QObject* object()=0;

    virtual QIcon icon() const=0;
    virtual bool hasUpdater() const=0;
    virtual void doUpdate()=0;
    virtual MainWindowBase* mainWindow() const=0;

protected:
    virtual void newDocument(DocumentBase*)=0;
};

Q_DECLARE_INTERFACE(PluginInterface, "org.wiiking2.PluginInterface/1.1a")
#endif // PLUGIN_HPP
