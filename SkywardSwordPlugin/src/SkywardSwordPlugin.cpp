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
#include "SettingsDialog.hpp"
#include "Constants.hpp"
#include <MainWindowBase.hpp>
#include <DocumentBase.hpp>
#include <Exception.hpp>

#include <QFileInfo>
#include <QIcon>
#include <QApplication>
#include <BinaryReader.hpp>
#include <Updater.hpp>
#include <QMessageBox>
#include <QDebug>

SkywardSwordPlugin* SkywardSwordPlugin::m_instance = NULL;
SkywardSwordPlugin::SkywardSwordPlugin()
    : m_enabled(true),
      m_icon(QIcon(":/icon/Bomb64x64.png")),
      m_settingsDialog(NULL),
      m_updater(new Updater(this))
{
    m_instance = this;
}

SkywardSwordPlugin::~SkywardSwordPlugin()
{
    if (m_updater)
        delete m_updater;
    if (m_settingsDialog)
        delete m_settingsDialog;
    if (settings())
        delete settings();
}

void SkywardSwordPlugin::initialize(MainWindowBase *mainWindow)
{
    m_mainWindow = mainWindow;

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
    return Constants::SKYWARDSWORD_TITLE;
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
    return "Plugin for (char)0xing and editing Skyward Sword save files <b>BETA</b>";
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

DocumentBase* SkywardSwordPlugin::loadFile(const QString& file) const
{
    SkywardSwordGameDocument* ret = new SkywardSwordGameDocument(this, file);
    if (ret->loadFile())
        return ret;

    delete ret;
    return NULL;
}

bool SkywardSwordPlugin::canLoad(const QString& filename)
{
    Int32 gameId = -1;
    if (QFileInfo(filename).suffix() == "bin")
    {
        try
        {
            zelda::io::BinaryReader reader(filename.toStdString());
            reader.setEndianess(zelda::BigEndian);
            if (reader.length() > 0xF0C0)
            {
                reader.seek(0xF124);
                gameId = reader.readUInt32();
                if ((gameId & 0xFFFFFF00) == 0x534F5500)
                    return true;
                else
                    gameId = -1;
            }
        }
        catch(...)
        {
            // Hide errors
        }
    }

    if (gameId == -1)
    {
        try
        {
            zelda::io::BinaryReader reader(filename.toStdString());
            reader.setEndianess(zelda::BigEndian);
            gameId = reader.readUInt32();

        }
        catch(zelda::error::Exception e)
        {
            // Hide errors
        }
    }

    if (gameId != -1 && ((gameId & 0xFFFFFF00) == 0x534F5500))
        return true;

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

    m_updateMBox.setWindowTitle(Constants::SKYWARDSWORD_UPDATE_CHECKING);
    m_updateMBox.setText(Constants::SKYWARDSWORD_UPDATE_CHECKING_MSG);
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

MainWindowBase* SkywardSwordPlugin::mainWindow() const
{
    return m_mainWindow;
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
