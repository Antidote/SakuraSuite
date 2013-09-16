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

#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP


// Need to add more settings

#include <QObject>
#include <QStringList>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    virtual ~SettingsManager();

    enum {NTSCU, NTSCJ, PAL};
    
    QString defaultPlayerNameForRegion(quint32 region) const;
    void setDefaultPlayerNameForRegion(quint32 region, const QString& name);
    void setDefaultPlayerNameForRegion(const QStringList&);

    QString defaultPlayerName() const;

    quint32 defaultRegion() const;
    void setDefaultRegion(const quint32 region);

    QString updateUrl() const;
    void setUpdateUrl(const QString& updateUrl);
    bool updateCheckOnStart() const;
    void setUpdateCheckOnStart(bool updateOnStart);
    void saveSettings();
    static SettingsManager* instance();
signals:
    
public slots:

private:
    SettingsManager* operator=(SettingsManager&);
    SettingsManager(SettingsManager&);
    SettingsManager();
    QStringList m_defaultNameList; //!< The default name for the region, Must be in order of NTSCU, NTSCJ, PAL
    quint32     m_defaultRegion;   //!< The default region for the save
    QString     m_updateUrl;
    bool        m_updateCheckOnStart;

    static SettingsManager* m_instance;
    static const ushort JAPANESE_NAME[4];
};

#endif // SETTINGSMANAGER_H
