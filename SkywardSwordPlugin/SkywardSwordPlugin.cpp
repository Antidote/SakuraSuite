#include "SkywardSwordPlugin.hpp"
#include "GameFile.hpp"
#include <QFileInfo>
#include <QDebug>

SkywardSwordPlugin::SkywardSwordPlugin()
{
    qDebug() << name() << "initialized";
}

SkywardSwordPlugin::~SkywardSwordPlugin()
{

}

QString SkywardSwordPlugin::filter() const
{
    return "Skyward Sword Saves *.sav (*.sav)";
}

QString SkywardSwordPlugin::extension() const
{
    return "sav";
}

QString SkywardSwordPlugin::name() const
{
    return "Skyward Sword Save Plugin";
}

QString SkywardSwordPlugin::author() const
{
    return "Phillip Stephens";
}

QString SkywardSwordPlugin::version() const
{
    return "1.0";
}

QString SkywardSwordPlugin::website() const
{
    return "http://wiiking2.com";
}

void SkywardSwordPlugin::setPath(const QString& path)
{
    if (!m_path.isEmpty())
        return;

    m_path = path;
}

QString SkywardSwordPlugin::path() const
{
    if (m_path.isEmpty())
        return "Unknown";
    return m_path;
}

QString SkywardSwordPlugin::license() const
{
    return "GPLv3";
}

QString SkywardSwordPlugin::description() const
{
    return "Plugin for loading and editing Skyward Sword save files <b>BETA</b>";
}

bool SkywardSwordPlugin::enabled() const
{
    return m_enabled;
}

void SkywardSwordPlugin::setEnabled(const bool enable)
{
    if (m_enabled == enable)
        return;

    m_enabled = enable;

    emit enabledChanged();
}

GameFile* SkywardSwordPlugin::loadFile(const QString& file) const
{
    return new GameFile(file);
}

bool SkywardSwordPlugin::canLoad(const QString& filename)
{
    return (!QString::compare(QFileInfo(filename).suffix(), extension(), Qt::CaseInsensitive));
}

QObject* SkywardSwordPlugin::object()
{
    return this;
}

Q_EXPORT_PLUGIN(SkywardSwordPlugin)
