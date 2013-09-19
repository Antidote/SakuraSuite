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

#include "MainWindow.hpp"
#include <QApplication>
#include "Constants.hpp"
#include <QTranslator>
#include <QDir>
#include <QDebug>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_WIN
    // This is unnecessary on any platform but Windows
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif
    a.setLibraryPaths(QStringList() << a.libraryPaths() << "plugins");
    a.setOrganizationName("org.wiiking2.com");
    a.setOrganizationDomain("http://wiiking2.com");
    a.setApplicationName(Constants::WIIKING2_APP_NAME);
    a.setApplicationVersion(Constants::WIIKING2_APP_VERSION);

    QTranslator appTranslator;
    appTranslator.load(a.applicationDirPath() + QDir::separator() + "lang" + QDir::separator() + QLocale::system().name());
    a.installTranslator(&appTranslator);
    MainWindow w;

    w.show();

    return a.exec();
}
