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

#include "SkywardSwordPlugin.hpp"
#include "SkywardSwordGameFile.hpp"
#include "SettingsManager.hpp"
#include "GameFile.hpp"
#include "SettingsDialog.hpp"
#include "Constants.hpp"

#include <QFileInfo>
#include <QIcon>
#include <QDebug>
#include <QApplication>
#include <BinaryReader.hpp>
#include <Updater.hpp>
#include <QMessageBox>

SkywardSwordPlugin* SkywardSwordPlugin::m_instance = NULL;
SkywardSwordPlugin::SkywardSwordPlugin()
    : m_icon(QIcon(":/icon/Bomb64x64.png")),
      m_updater(new Updater(this)),
      m_enabled(true)
{
    m_instance = this;
}

SkywardSwordPlugin::~SkywardSwordPlugin()
{
    delete m_updater;
    delete m_settingsDialog;
    delete settings();
}

void SkywardSwordPlugin::initialize()
{
    m_settingsDialog = new SettingsDialog(qApp->topLevelWidgets()[0]);
    connect(m_updater, SIGNAL(done()), this, SLOT(onUpdaterDone()));
    connect(m_updater, SIGNAL(error(Updater::ErrorType)), this, SLOT(onUpdaterError(Updater::ErrorType)));
    connect(m_updater, SIGNAL(noUpdate()), this, SLOT(onNoUpdate()));
    connect(m_updater, SIGNAL(warning(QString)), this, SLOT(onUpdaterWarning(QString)));

    if (settings()->updateCheckOnStart())
        doUpdate();
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
    return Constants::SKYWARDSWORD_PLUGIN_NAME;
}

QString SkywardSwordPlugin::author() const
{
    return "Phillip Stephens";
}

QString SkywardSwordPlugin::version() const
{
    return Constants::SKYWARDSWORD_PLUGIN_VERSION;
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

        if (magic == 0x534F5500)
            return true;
    }
    catch(...)
    {
        // hide errors
    }

    return false;
}

bool SkywardSwordPlugin::hasUpdater() const
{
    return true;
}

void SkywardSwordPlugin::doUpdate()
{
    QSettings settings;
    settings.beginGroup(Constants::SKYWARDSWORD_PLUGIN_NAME);
    if (!settings.value(Constants::Settings::SKYWARDSWORD_UPDATE_URL).isValid())
        settings.setValue(Constants::Settings::SKYWARDSWORD_UPDATE_URL, Constants::Settings::SKYWARDSWORD_UPDATE_URL_DEFAULT);

    if (!m_updateMBox.parent())
        m_updateMBox.setParent((QWidget*)parent());

    m_updateMBox.setWindowTitle("Please wait...");
    m_updateMBox.setText("Checking for updates, please wait.");
    m_updateMBox.setStandardButtons(QMessageBox::NoButton);
    // This prevents the user from clicking away
    m_updateMBox.setWindowModality(Qt::WindowModal);

    // Prevent the user from interrupting the check
    ((QWidget*)parent())->setEnabled(false);
    // Using exec blocks everything, is there a work around?
    m_updateMBox.show();

#ifdef SS_INTERNAL
    m_updater->checkForUpdate(Constants::Settings::SKYWARDSWORD_UPDATE_URL_DEFAULT, Constants::SKYWARDSWORD_PLUGIN_VERSION, Constants::SKYWARDSWORD_VERSION);
#else
    m_updater->checkForUpdate(settings.value(Constants::Settings::SKYWARDSWORD_UPDATE_URL).toString(), Constants::SKYWARDSWORD_PLUGIN_VERSION, Constants::SKYWARDSWORD_VERSION);
#endif
}

Updater* SkywardSwordPlugin::updater()
{
    return m_updater;
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

void SkywardSwordPlugin::onUpdaterDone()
{
    ((QWidget*)parent())->setEnabled(true);
    m_updateMBox.hide();
    m_updateMBox.setWindowTitle(Constants::SKYWARDSWORD_NOT_LATEST_VERSION);
    m_updateMBox.setText(Constants::SKYWARDSWORD_NOT_LATEST_VERSION_MSG
                         .arg(Constants::SKYWARDSWORD_PLUGIN_NAME)
                         .arg(m_updater->updateUrl())
                         .arg(m_updater->md5Sum())
                         .arg(m_updater->changelogUrl()));

    m_updateMBox.setStandardButtons(QMessageBox::Ok);
    m_updateMBox.exec();
}

void SkywardSwordPlugin::onUpdaterError(Updater::ErrorType et)
{
    ((QWidget*)parent())->setEnabled(true);
    m_updateMBox.hide();
    m_updateMBox.setStandardButtons(QMessageBox::Ok);
    m_updateMBox.setWindowModality(Qt::NonModal);

    switch(et)
    {
        case Updater::UnableToConnect:
            m_updateMBox.setWindowTitle(Constants::SKYWARDSWORD_UPDATE_CONTACT_ERROR);
            m_updateMBox.setText(Constants::SKYWARDSWORD_UPDATE_CONTACT_ERROR_MSG);
            break;
        case Updater::ParseError:
            m_updateMBox.setWindowTitle(Constants::SKYWARDSWORD_UPDATE_PARSE_ERROR);
            m_updateMBox.setText(Constants::SKYWARDSWORD_UPDATE_PARSE_ERROR_MSG.arg(m_updater->errorString()));
            break;
        case Updater::NoPlatform:
            m_updateMBox.setWindowTitle(Constants::SKYWARDSWORD_UPDATE_PLATFORM);
            m_updateMBox.setText(Constants::SKYWARDSWORD_UPDATE_PLATFORM_MSG);
            break;
    }
    m_updateMBox.exec();
}

void SkywardSwordPlugin::onUpdaterWarning(QString warning)
{
    qWarning() << warning;
    m_updateMBox.setText(m_updateMBox.text() + "<br />" + warning);
}

void SkywardSwordPlugin::onNoUpdate()
{
    ((QWidget*)parent())->setEnabled(true);
    m_updateMBox.hide();
    m_updateMBox.setWindowTitle(Constants::SKYWARDSWORD_LATEST_VERSION);
    m_updateMBox.setText(Constants::SKYWARDSWORD_LATEST_VERSION_MSG);
    m_updateMBox.setStandardButtons(QMessageBox::Ok);
    m_updateMBox.setWindowModality(Qt::NonModal);
    m_updateMBox.exec();
}

Q_EXPORT_PLUGIN(SkywardSwordPlugin)
