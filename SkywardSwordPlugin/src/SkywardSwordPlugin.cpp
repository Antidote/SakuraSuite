#include "SkywardSwordPlugin.hpp"
#include "SkywardSwordGameFile.hpp"
#include "SettingsManager.hpp"
#include "GameFile.hpp"
#include "SettingsDialog.hpp"
#include <QFileInfo>
#include <QIcon>
#include <QDebug>
#include <BinaryReader.hpp>

SkywardSwordPlugin* SkywardSwordPlugin::m_instance = NULL;
SkywardSwordPlugin::SkywardSwordPlugin()
    : m_icon(QIcon(":/icon/Bomb64x64.png"))
{
    m_instance = this;
    m_settingsDialog = new SettingsDialog;
    qDebug() << name() << "initialized";
}

SkywardSwordPlugin::~SkywardSwordPlugin()
{
    delete settings();
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
    return new SkywardSwordGameFile(this, file);
}

bool SkywardSwordPlugin::canLoad(const QString& filename)
{
    try
    {
        zelda::io::BinaryReader reader(filename.toStdString());
        reader.setEndianess(zelda::BigEndian);
        Uint32 magic = reader.readUInt32() & 0xFFFFFF00;
        qDebug() << hex << magic;

        if (magic == 0x534F5500)
            return true;
    }
    catch(...)
    {
        // hide errors
    }

    return false;
}

QDialog* SkywardSwordPlugin::settingsDialog()
{
    return m_settingsDialog;
}

QObject* SkywardSwordPlugin::object()
{
    return this;
}

QIcon SkywardSwordPlugin::icon() const
{
    return m_icon;
}

SkywardSwordPlugin* SkywardSwordPlugin::instance()
{
    return m_instance;
}

SettingsManager* SkywardSwordPlugin::settings()
{
    return SettingsManager::instance();
}

Q_EXPORT_PLUGIN(SkywardSwordPlugin)
