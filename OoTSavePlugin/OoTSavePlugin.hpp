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

#ifndef OOTSAVEPLUGIN_HPP
#define OOTSAVEPLUGIN_HPP

#include "ootsaveplugin_global.hpp"
#include <QObject>
#include <QIcon>
#include "PluginInterface.hpp"

class OOTSAVEPLUGINSHARED_EXPORT OoTSavePlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    OoTSavePlugin();
    ~OoTSavePlugin();

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

    GameFile* loadFile(const QString& file) const;
    bool canLoad(const QString& filename);

    QDialog* settingsDialog();
    QObject* object();

    QIcon icon() const;
signals:
    void enabledChanged();
public slots:

private:
    bool m_enabled;
    QString m_path;
    QIcon m_icon;
};

#endif // OOTSAVEPLUGIN_HPP
