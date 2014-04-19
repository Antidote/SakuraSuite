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
#include <ApplicationLog.hpp>
#ifdef Q_OS_WIN
#include <QCommandLineParser>
#include <QCommandLineOption>
#endif

#include "Constants.hpp"
#include <QTranslator>
#include <QDir>
#include <QLocale>
#include <QDebug>
#include <QProcess>

void messageHander(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
    QString formattedMsg = QString("[%1(%2) %3]: %4").arg(context.file).arg(context.line).arg(context.function).arg(msg);
    switch(type)
    {
        case QtDebugMsg:
            ApplicationLog::instance()->debug(formattedMsg);
            break;
        case QtWarningMsg:
            ApplicationLog::instance()->warning(formattedMsg);
            break;
        case QtCriticalMsg:
            ApplicationLog::instance()->error(formattedMsg);
            break;
        case QtFatalMsg:
            ApplicationLog::instance()->fatal(formattedMsg);
            break;
    }
}

int main(int argc, char *argv[])
{
    try
    {
        QApplication a(argc, argv);
        qInstallMessageHandler(messageHander);
        qDebug() << "Starting...";
        a.setLibraryPaths(QStringList() << a.libraryPaths() << "plugins");
        a.setOrganizationName("org.wiiking2.com");
        a.setOrganizationDomain("http://wiiking2.com");
        a.setApplicationName(Constants::SAKURASUITE_APP_NAME);
        a.setApplicationVersion(Constants::SAKURASUITE_APP_VERSION);
        a.setWindowIcon(QIcon(":/about/SakuraSuite.png"));

        if (!QDir(QDir::homePath() + ".sakurasuite").exists())
            QDir(QDir::homePath()).mkdir(".sakurasuite");

#ifdef Q_OS_WIN
        QCommandLineParser parser;
        QCommandLineOption iconThemeOption("icon-theme", "Set applications icon theme", "icon-theme");
        parser.addOption(iconThemeOption);
        parser.setApplicationDescription(QString("%1 v%2")
                                         .arg(Constants::SAKURASUITE_TITLE)
                                         .arg(Constants::SAKURASUITE_APP_VERSION));
        QCommandLineOption helpOption = parser.addHelpOption();
        // This is unnecessary on any platform but Windows
        QSettings::setDefaultFormat(QSettings::IniFormat);
        iconThemeOption.setDefaultValue("oxygen");
        parser.process(a);

        if (parser.isSet(helpOption))
        {
            QMessageBox helpMessage;
            helpMessage.setWindowTitle("Help...");
            QString helpString = parser.helpText().toUtf8();
            QString path =  a.applicationDirPath() + QDir::separator();
            helpMessage.setText(helpString.remove(path.replace("/", "\\")));
            helpMessage.exec();
            parser.showHelp();
        }

        if (parser.value(iconThemeOption) != QString())
            QIcon::setThemeName(parser.value(iconThemeOption));
        else
            QIcon::setThemeName(iconThemeOption.defaultValues().first());

#endif

        qDebug() << "Initializing translator...";
        QTranslator appTranslator;
        appTranslator.load(a.applicationDirPath() + QDir::separator() + "lang" + QDir::separator() + QLocale::system().name());
        a.installTranslator(&appTranslator);
        qDebug() << "Creating MainWindow...";
        MainWindow w;

        w.show();

        return a.exec();
    }
    catch(...)
    {
        return 1;
    }
}
