// This file is part of Sakura Suite.
//
// Sakura Suite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Sakura Suite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura Suite.  If not, see <http://www.gnu.org/licenses/>

#include "MainWindow.hpp"
#include <QApplication>
#include "Constants.hpp"
#include <QTranslator>
#include <QDir>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_WIN
    // This is unnecessary on any platform but Windows
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif
    if (!QDir(QDir::homePath() + ".sakurasuite").exists())
        QDir(QDir::homePath()).mkdir(".sakurasuite");

    a.setLibraryPaths(QStringList() << a.libraryPaths() << "plugins");
    a.setOrganizationName("org.wiiking2.com");
    a.setOrganizationDomain("http://wiiking2.com");
    a.setApplicationName(Constants::SAKURASUITE_APP_NAME);
    a.setApplicationVersion(Constants::SAKURASUITE_APP_VERSION);
    a.setWindowIcon(QIcon(":/about/SakuraSuite.png"));

    QTranslator appTranslator;
    appTranslator.load(a.applicationDirPath() + QDir::separator() + "lang" + QDir::separator() + QLocale::system().name());
    a.installTranslator(&appTranslator);
    MainWindow w;

    w.show();

    return a.exec();
}
