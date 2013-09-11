#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <QSettings>

// #define WK2_INTERNAL
#define WK2_PREVIEW
namespace Constants
{
#define tr QObject::tr
const int     WIIKING2_MAJOR                    = 0;
const int     WIIKING2_MINOR                    = 1;
const int     WIIKING2_PATCH                    = 0;
const int     WIIKING2_VERSION                  = (WIIKING2_MAJOR << 16) | (WIIKING2_MINOR << 8) | WIIKING2_PATCH;
const QString WIIKING2_VERSION_STR              = QString("%1.%2.%3").arg(WIIKING2_MAJOR)
                                                                     .arg(WIIKING2_MINOR)
                                                                     .arg(WIIKING2_PATCH);

const QString WIIKING2_TITLE                    = tr("WiiKing2 Editor");
const QString WIIKING2_TITLE_FILE               = tr("%1%2 - Wiiking2 Editor");
const QString WIIKING2_TITLE_DIRTY              = tr("*");
const QString WIIKING2_APP_NAME                 = tr("Wiiking2Editor");

#ifdef WK2_DEBUG

#ifdef WK2_INTERNAL
const QString WIIKING2_APP_VERSION            = QString(WIIKING2_VERSION_STR + " INTERNAL");
#elif defined(WK2_PREVIEW)
const QString WIIKING2_APP_VERSION            = QString(WIIKING2_VERSION_STR + " PREVIEW");
#else
const QString WIIKING2_APP_VERSION            = QString(WIIKING2_VERSION_STR + " DEBUG");
#endif // INTERNAL

#else
const QString WIIKING2_APP_VERSION            = QString(WIIKING2_VERSION_STR);
#endif

const QString WIIKING2_UPDATE_ERROR          = tr("Unable to contact server");
const QString WIIKING2_UPDATE_ERROR_MSG      = tr("%1 was unable to contact the update server.<br />"
                                                  "check your network settings and try again.<br />"
                                                  "If the issue persists, the server may not be available <br/>"
                                                  "in that case try again later or contact a developer for assistance");
const QString WIIKING2_LATEST_VERSION           = tr("At latest version...");
const QString WIIKING2_LATEST_VERSION_MSG       = tr("The application is up to date.");
const QString WIIKING2_NOT_LATEST_VERSION       = tr("%1 is out of date...");
const QString WIIKING2_NOT_LATEST_VERSION_MSG   = tr("An update is available for %1<br />"
                                               "You may download the update at <a href=\"%2\">%2</a><br />"
                                               "The changelog is available at <a href=\"%3\">%3</a>");

const QString WIIKING2_BUILD_DATE               = tr(__DATE__ " " __TIME__);
const QString WIIKING2_NO_PLUGINS_ERROR         = tr("Error Loading Plugins...");
const QString WIIKING2_NO_PLUGINS_ERROR_MSG   = tr("No plugins were loaded.\n"
                                               "Please check the plugins directory in the application's root");
const QString WIIKING2_PLUGIN_RELOAD_WARNING  = tr("Reload plugin?");
const QString WIIKING2_PLUGIN_RELOAD_WARNING_MSG  = tr("You will lose <b>all</b> unsaved data associated with this plugin<br />"
                                                   "Are you sure you wish to reload?");
const QString WIIKING2_ABOUT_TITLE            = tr("About %1...");
const QString WIIKING2_ERROR_LOADING          = tr("Error loading file...");
const QString WIIKING2_ERROR_LOADING_MSG      = tr("Error loading file '%1' please check that it exists");
const QString WIIKING2_DIRTY_NAG              = tr("File has been changed...");
const QString WIIKING2_DIRTY_NAG_MSG          = tr("The file '%1' has been modified, do you wish to save?");
const QString WIIKING2_ALREADY_OPENED_NAG     = tr("Already Opened...");
const QString WIIKING2_ALREADY_OPENED_NAG_MSG = tr("The file '%1' is already opened and will not be reopened");


namespace Settings
{
const QString WIIKING2_RECENT_FILES           = QString("recentFiles");
const QString WIIKING2_RECENT_DIRECTORY       = QString("recentDirectory");
}

#undef tr
}
#endif // CONSTANTS_HPP
