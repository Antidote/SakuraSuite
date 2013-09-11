#include "OoTSavePlugin.hpp"
#include "GameFile.hpp"


OoTSavePlugin::OoTSavePlugin()
    : m_icon(QIcon(":/icon/Ocarina64x64.png"))
{
}

OoTSavePlugin::~OoTSavePlugin()
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

GameFile* OoTSavePlugin::loadFile(const QString& file) const
{
    return new GameFile(this, file);
}

bool OoTSavePlugin::canLoad(const QString& filename)
{
    return (!QString::compare(QFileInfo(filename).suffix(), extension(), Qt::CaseInsensitive));
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
