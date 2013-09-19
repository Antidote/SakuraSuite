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

#include <GameDocument.hpp>
#include <QObject>

class CopyWidget;
class SkywardSwordEditorForm;

class SkywardSwordGameDocument : public GameDocument
{
    Q_OBJECT
public:
    SkywardSwordGameDocument(const PluginInterface* loader, const QString& file = QString());
    ~SkywardSwordGameDocument();
    QString game() const;
    bool save(const QString &filename = QString());

    bool loadFile();

    bool reload();
private slots:
    void onModified();
    void onCopy(SkywardSwordEditorForm* source);
    void onTabMoved(int left, int right);
private:
    char*       m_skipData;
    char        m_region;
    CopyWidget* m_copyWidget;
};

#endif // SKYWARDSWORDGAMEFILE_HPP
