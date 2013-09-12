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

#ifndef SKYWARDSWORDGAMEFILE_HPP
#define SKYWARDSWORDGAMEFILE_HPP

#include <GameFile.hpp>
#include <QObject>

class SkywardSwordGameFile : public GameFile
{
    Q_OBJECT
public:
    SkywardSwordGameFile(const PluginInterface* loader, const QString& file = QString());

    QString game() const;
    bool save(const QString &filename = QString());

private slots:
    void onModified();
private:
    char* m_skipData;
    char m_region;
};

#endif // SKYWARDSWORDGAMEFILE_HPP
