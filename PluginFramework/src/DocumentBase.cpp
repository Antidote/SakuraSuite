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

#include "DocumentBase.hpp"
#include <QTabWidget>
#include <QFileInfo>
#include <QDir>

DocumentBase::DocumentBase(const PluginInterface* loader, const QString &file)
    : m_loader((PluginInterface*)loader),
      m_dirty(false),
      m_widget(NULL)
{
    if (!file.isEmpty())
    {
        QFileInfo fInfo(file);
        m_file = fInfo.fileName();
        m_path = fInfo.absolutePath();
    }
}

DocumentBase::~DocumentBase()
{
    delete m_widget;
    m_widget = NULL;
}


QWidget* DocumentBase::widget() const
{
    return (QWidget*)m_widget;
}

QString DocumentBase::fileName() const
{
    return m_file;
}

QString DocumentBase::fileDir() const
{
    return m_path;
}

QString DocumentBase::filePath() const
{
    if (!m_path.isEmpty() && !m_file.isEmpty())
        return m_path + "/" + m_file;

    return QString();
}

PluginInterface* DocumentBase::loadedBy()
{
    return m_loader;
}

void DocumentBase::setDirty(bool dirty)
{
    m_dirty = dirty;
}

bool DocumentBase::isDirty()
{
    return m_dirty;
}

bool DocumentBase::save(const QString &filename)
{
    Q_UNUSED(filename);
    return false;
}

bool DocumentBase::reload()
{
    return true;
}
