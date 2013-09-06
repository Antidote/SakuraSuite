#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <QSettings>

namespace Constants
{
const char WIIKING2_TITLE[]                  = "WiiKing2 Editor";
const char WIIKING2_TITLE_FILE[]             = "%1%2 - Wiiking2 Editor";
const char WIIKING2_TITLE_DIRTY              = '*';
const char WIIKING2_APP_NAME[]               = "Wiiking2Editor";

#ifdef WK2_DEBUG

#ifdef WK2_INTERNAL
const char WIIKING2_APP_VERSION[]            = "INTERNAL";
#else
const char WIIKING2_APP_VERSION[]            = "0.1 DEBUG";
#endif // INTERNAL

#else
const char WIIKING2_APP_VERSION[]            = "0.1";
#endif

const char WIIKING2_NO_PLUGINS_ERROR[]       = "Error Loading Plugins...";
const char WIIKING2_NO_PLUGINS_ERROR_MSG[]   = "No plugins were loaded.\n"
                                               "Please check the plugins directory in the application's root";
const char WIIKING2_ABOUT_TITLE[]            = "About %1...";
const char WIIKING2_ERROR_LOADING[]          = "Error loading file...";
const char WIIKING2_ERROR_LOADING_MSG[]      = "Error loading file '%1' please check that it exists";
const char WIIKING2_DIRTY_NAG[]              = "File has been changed...";
const char WIIKING2_DIRTY_NAG_MSG[]          = "The file '%1' has been modified, do you wish to save?";
const char WIIKING2_ALREADY_OPENED_NAG[]     = "Already Opened...";
const char WIIKING2_ALREADY_OPENED_NAG_MSG[] = "The file '%1' is already opened and will not be reopened";


namespace Settings
{
const char WIIKING2_RECENT_FILES[]           = "recentFiles";
const char WIIKING2_RECENT_DIRECTORY[]       = "recentDirectory";
}
}
#endif // CONSTANTS_HPP
