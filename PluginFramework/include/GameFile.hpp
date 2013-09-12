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

#ifndef GAMEFILE_HPP
#define GAMEFILE_HPP

#include <QWidget>
class PluginInterface;

class GameFile : public QObject
{
    Q_OBJECT
public:
    GameFile(const PluginInterface* loader, const QString& file = QString());
    virtual ~GameFile();

    QWidget* widget() const;

    /// \brief Name of file without path.
    QString fileName() const;
    /// \brief Absolute path to file, excluding filename.
    QString fileDir() const;
    /// \brief Absolute path to file, including filename.
    QString filePath() const;

    PluginInterface* loadedBy();
    void setDirty(bool dirty);
    virtual bool isDirty();

    virtual QString game() const;

    virtual bool save(const QString& filename = QString());

signals:
    void modified();
    void enabledChanged();
protected:
    PluginInterface* m_loader;
    bool     m_dirty;
    QString  m_file;
    QString  m_path;
    QWidget* m_widget;
};

#endif // GAMEFILE_HPP
