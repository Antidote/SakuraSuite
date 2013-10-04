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

#ifndef DOCUMENTBASE_HPP
#define DOCUMENTBASE_HPP

#include <QWidget>
class PluginInterface;

class DocumentBase : public QObject
{
    Q_OBJECT
public:
    DocumentBase(const PluginInterface* loader, const QString& file = QString());
    virtual ~DocumentBase();

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

    virtual bool save(const QString& filename = QString());
    virtual bool reload();
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

#endif // DOCUMENTBASE_HPP
