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

#include "OoTSavePlugin.hpp"
#include "DocumentBase.hpp"


OoTSavePlugin::OoTSavePlugin()
    : m_icon(QIcon(":/icon/Ocarina64x64.png")),
      m_enabled(true)
{
}

OoTSavePlugin::~OoTSavePlugin()
{

}

void OoTSavePlugin::initialize()
{
}

QString OoTSavePlugin::filter() const
{
    return "Ocarina of Times *.sra (*.sra)";
}

QString OoTSavePlugin::extension() const
{
    return "sra";
}

QString OoTSavePlugin::name() const
{
    return "Ocarina of Time Save Plugin";
}

QString OoTSavePlugin::author() const
{
    return "Phillip Stephens";
}

QString OoTSavePlugin::version() const
{
    return "1.0";
}

QString OoTSavePlugin::website() const
{
    return "http://wiiking2.com";
}

void OoTSavePlugin::setPath(const QString& path)
{
    if (!m_path.isEmpty())
        return;

    m_path = path;
}

QString OoTSavePlugin::path() const
{
    if (m_path.isEmpty())
        return "Unknown";
    return m_path;
}

QString OoTSavePlugin::license() const
{
    return "GPLv3";
}

QString OoTSavePlugin::description() const
{
    return "Plugin for loading and editing Ocarina of Time save files <b>BETA</b>";
}

bool OoTSavePlugin::enabled() const
{
    return m_enabled;
}

void OoTSavePlugin::setEnabled(const bool enable)
{
    if (m_enabled == enable)
        return;

    m_enabled = enable;

    emit enabledChanged();
}

DocumentBase* OoTSavePlugin::loadFile(const QString& file) const
{
    return new DocumentBase(this, file);
}

bool OoTSavePlugin::canLoad(const QString& filename)
{
    return (!QString::compare(QFileInfo(filename).suffix(), extension(), Qt::CaseInsensitive));
}

bool OoTSavePlugin::hasUpdater() const
{
    return false;
}

void OoTSavePlugin::doUpdate()
{

}

Updater* OoTSavePlugin::updater()
{
    return NULL;
}

QDialog* OoTSavePlugin::settingsDialog()
{
    return NULL;
}

QObject* OoTSavePlugin::object()
{
    return this;
}

QIcon OoTSavePlugin::icon() const
{
    return m_icon;
}

Q_EXPORT_PLUGIN(OoTSavePlugin)
