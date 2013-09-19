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

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <QObject>

namespace Constants
{
const QString UPDATER_VERSION_REGEXP       = QString("^version=");
const QString UPDATER_VERSION_STRING_REGEXP= QString("^versionstring=");
const QString UPDATER_CHANGELOG_REGEXP     = QString("^changelog=");
const QString UPDATER_MD5_REGEXP           = QString("^%1-md5=");
const QString UPDATER_URL_REGEXP           = QString("^%1=");
}
#endif // CONSTANTS_HPP
