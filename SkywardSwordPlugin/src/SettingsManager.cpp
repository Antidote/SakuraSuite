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

#include "SettingsManager.hpp"
#include "SkywardSwordPlugin.hpp"
#include "Constants.hpp"
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QApplication>

SettingsManager* SettingsManager::m_instance = NULL;
const ushort     SettingsManager::JAPANESE_NAME[4] = {0x30ea, 0x30f3, 0x30af, 0};

SettingsManager::SettingsManager()
    : QObject(NULL)
{
    m_defaultNameList = QStringList() << "Link" << QString::fromUtf16(JAPANESE_NAME) << "Link";
    m_defaultRegion   = NTSCU;

    QSettings settings;
    settings.beginGroup(SkywardSwordPlugin::instance()->name());
    m_defaultNameList = settings.value("defaultPlayerNameForRegion", m_defaultNameList).toStringList();
    m_defaultRegion = (settings.value("defaultRegion", "NTSCU") == "NTSCU" ? NTSCU
                                                                           : settings.value("defaultRegion", "NTSCJ") == "NTSCJ"? NTSCJ : PAL);

#ifdef SS_INTERNAL
    m_updateUrl = Constants::Settings::SKYWARDSWORD_UPDATE_URL_DEFAULT;
#else
    m_updateUrl = settings.value(Constants::Settings::SKYWARDSWORD_UPDATE_URL, Constants::Settings::SKYWARDSWORD_UPDATE_URL_DEFAULT).toString();
#endif

    m_updateCheckOnStart = settings.value(Constants::Settings::SKYWARDSWORD_CHECK_ON_START, false).toBool();
    settings.endGroup();
}

SettingsManager::~SettingsManager()
{
    saveSettings();
}


QString SettingsManager::defaultPlayerNameForRegion(quint32 region) const
{
    return m_defaultNameList[region];
}

void SettingsManager::setDefaultPlayerNameForRegion(quint32 region, const QString &name)
{
    m_defaultNameList[region] = name;
    saveSettings();
}

QString SettingsManager::defaultPlayerName() const
{
    return m_defaultNameList[m_defaultRegion];
}

quint32 SettingsManager::defaultRegion() const
{
    return m_defaultRegion;
}

void SettingsManager::setDefaultRegion(const quint32 region)
{
    if (region > PAL)
        return;

    m_defaultRegion = region;
    saveSettings();
}

QString SettingsManager::updateUrl() const
{
    return m_updateUrl;
}

void SettingsManager::setUpdateUrl(const QString& updateUrl)
{
    m_updateUrl = updateUrl;
    saveSettings();
}

bool SettingsManager::updateCheckOnStart() const
{
    return m_updateCheckOnStart;
}

void SettingsManager::setUpdateCheckOnStart(bool updateOnStart)
{
    m_updateCheckOnStart = updateOnStart;
    saveSettings();
}

void SettingsManager::saveSettings()
{
    QSettings settings;
    settings.beginGroup(SkywardSwordPlugin::instance()->name());
    settings.setValue("defaultPlayerNameForRegion", m_defaultNameList);
    settings.setValue("defaultRegion", (m_defaultRegion == NTSCU ? "NTSCU" : (m_defaultRegion == NTSCJ ? "NTSCJ" : "PAL")));
#ifndef SS_INTERNAL
    settings.setValue(Constants::Settings::SKYWARDSWORD_UPDATE_URL, m_updateUrl);
#endif

    settings.setValue(Constants::Settings::SKYWARDSWORD_CHECK_ON_START, m_updateCheckOnStart);
    settings.endGroup();
}


SettingsManager *SettingsManager::instance()
{
    if (!m_instance)
        m_instance = new SettingsManager;

    return m_instance;
}
