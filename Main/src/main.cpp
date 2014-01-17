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
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "Constants.hpp"
#include <QTranslator>
#include <QDir>
#include <QLocale>
#include <QDebug>

int main(int argc, char *argv[])
{
    try
    {
        QCommandLineParser parser;
        QCommandLineOption iconThemeOption("icon-theme", "Set applications icon theme", "icon-theme");
        parser.addOption(iconThemeOption);
        parser.setApplicationDescription(QString("%1 v%2")
                                         .arg(Constants::SAKURASUITE_TITLE)
                                         .arg(Constants::SAKURASUITE_APP_VERSION));
        QCommandLineOption helpOption = parser.addHelpOption();

        QApplication a(argc, argv);
        a.setLibraryPaths(QStringList() << a.libraryPaths() << "plugins");
        a.setOrganizationName("org.wiiking2.com");
        a.setOrganizationDomain("http://wiiking2.com");
        a.setApplicationName(Constants::SAKURASUITE_APP_NAME);
        a.setApplicationVersion(Constants::SAKURASUITE_APP_VERSION);
        a.setWindowIcon(QIcon(":/about/SakuraSuite.png"));

#ifdef Q_OS_WIN
        // This is unnecessary on any platform but Windows
        QSettings::setDefaultFormat(QSettings::IniFormat);
        iconThemeOption.setDefaultValue("oxygen");
#endif
        parser.process(a);

        if (parser.isSet(helpOption))
        {
            QMessageBox helpMessage;
            helpMessage.setWindowTitle("Help...");
            QString helpString = parser.helpText().toUtf8();
            QString path =  a.applicationDirPath() + QDir::separator();
#ifdef Q_OS_WIN
            path = path.replace("/", "\\");
#endif
            helpMessage.setText(helpString.remove(path));
            helpMessage.exec();
            parser.showHelp();
        }

        if (parser.value(iconThemeOption) != QString())
            QIcon::setThemeName(parser.value(iconThemeOption));
        else
            QIcon::setThemeName(iconThemeOption.defaultValues().first());

        if (!QDir(QDir::homePath() + ".sakurasuite").exists())
            QDir(QDir::homePath()).mkdir(".sakurasuite");

        QTranslator appTranslator;
        appTranslator.load(a.applicationDirPath() + QDir::separator() + "lang" + QDir::separator() + QLocale::system().name());
        a.installTranslator(&appTranslator);
        MainWindow w;

        w.show();

        return a.exec();
    }
    catch(...)
    {
        return 1;
    }
}
