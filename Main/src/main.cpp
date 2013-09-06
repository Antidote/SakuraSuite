#include "MainWindow.hpp"
#include <QApplication>
#include "Constants.hpp"
#include <QTranslator>
#include <QDir>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("org.wiiking2.com");
    a.setOrganizationDomain("http://wiiking2.com");
    a.setApplicationName(Constants::WIIKING2_APP_NAME);
    a.setApplicationVersion(Constants::WIIKING2_APP_VERSION);
    //a.setAPp(Constants::WIIKING2_TITLE);

    QTranslator appTranslator;
    appTranslator.load(a.applicationDirPath() + QDir::separator() + "lang" + QDir::separator() + QLocale::system().name());
    a.installTranslator(&appTranslator);
    MainWindow w;
    // This is unnecessary on any platform but Windows
#ifdef Q_OS_WIN32
    Constants::Settings.setDefaultFormat(QSettings::IniFormat);
#endif
    w.show();

    return a.exec();
}
