#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


#include <QSettings>

// Only ONE of these may be defined
//#define SS_PREVIEW
#define SS_INTERNAL

#if defined(SS_PREVIEW) && defined(SS_INTERNAL)
#error "You can only define either SS_PREVIEW or SS_INTERNAL"
#endif


namespace Constants
{
#define tr QObject::tr

const int     SKYWARDSWORD_MAJOR                     = 0;
const int     SKYWARDSWORD_MINOR                     = 1;
const int     SKYWARDSWORD_PATCH                     = 1;
const int     SKYWARDSWORD_VERSION                   = (SKYWARDSWORD_MAJOR << 16) | (SKYWARDSWORD_MINOR << 8) | SKYWARDSWORD_PATCH;
const QString SKYWARDSWORD_VERSION_STR               = QString("%1.%2.%3").arg(SKYWARDSWORD_MAJOR)
                                                                          .arg(SKYWARDSWORD_MINOR)
                                                                          .arg(SKYWARDSWORD_PATCH);
#ifdef SS_PREVIEW
const QString SKYWARDSWORD_PLUGIN_VERSION            = QString(SKYWARDSWORD_VERSION_STR + " PREVIEW 2");
#elif defined(SS_INTERNAL)
const QString SKYWARDSWORD_PLUGIN_VERSION            = QString(SKYWARDSWORD_VERSION_STR + " INTERNAL");
#elif defined(DEBUG)
const QString SKYWARDSWORD_PLUGIN_VERSION            = QString(SKYWARDSWORD_VERSION_STR + " DEBUG");
#else
const QString SKYWARDSWORD_PLUGIN_VERSION            = QString(SKYWARDSWORD_VERSION_STR);
#endif

const QString SKYWARDSWORD_TITLE                     = QString("Skyward Sword Plugin");
const QString SKYWARDSWORD_PLUGIN_NAME               = QString("skywardswordplugin");
const QString SKYWARDSWORD_UPDATE_CHECKING           = tr("Checking for updates please wait...");
const QString SKYWARDSWORD_UPDATE_CHECKING_MSG       = tr("%1 is checking for updates, please wait.").arg(SKYWARDSWORD_TITLE);
const QString SKYWARDSWORD_UPDATE_CONTACT_ERROR      = tr("Unable to contact server");
const QString SKYWARDSWORD_UPDATE_CONTACT_ERROR_MSG  = tr("%1 was unable to contact the update server.<br />"
                                                          "check your network settings and try again.<br />"
                                                          "If the issue persists, the server may not be available <br/>"
                                                          "in that case try again later or contact a developer for assistance").arg(SKYWARDSWORD_TITLE);
const QString SKYWARDSWORD_UPDATE_PARSE_ERROR        = tr("Error while parsing...");
const QString SKYWARDSWORD_UPDATE_PARSE_ERROR_MSG    = tr("Unable to parse update file <br />"
                                                          "Please contact a developer for assistance with this message:<br />%1");
const QString SKYWARDSWORD_LATEST_VERSION            = tr("%1 is up to date...").arg(SKYWARDSWORD_TITLE);
const QString SKYWARDSWORD_LATEST_VERSION_MSG        = tr("%1 is at the latest version.<br />"
                                                          "If you feel this is an error please contact a developer with your platform.").arg(SKYWARDSWORD_TITLE);
const QString SKYWARDSWORD_NOT_LATEST_VERSION        = tr("%1 is out of date...").arg(SKYWARDSWORD_TITLE);
const QString SKYWARDSWORD_NOT_LATEST_VERSION_MSG    = tr("An update is available for %1<br />"
                                                          "You may download the update at: <a href=\"%2\">%2</a><br />"
                                                          "MD5 %3<br />"
                                                          "The changelog is available at: <a href=\"%4\">%4</a>");
const QString SKYWARDSWORD_UPDATE_PLATFORM           = tr("Unsupported Platform...");
const QString SKYWARDSWORD_UPDATE_PLATFORM_MSG       = tr("The updater currently does not support your platform.<br />"
                                                          "If you are using an unofficial build, we do not provide support.<br />"
                                                          "We appologize for the inconvenience."
                                                          "<br />"
                                                          "If you think this is in error, please contact a developer with your platform.");

namespace Settings
{
#ifdef SS_INTERNAL
const QString SKYWARDSWORD_UPDATE_URL_DEFAULT = "http://update.local/skywardswordplugin/latest.update";
#else
const QString SKYWARDSWORD_UPDATE_URL_DEFAULT = "http://update.wiiking2.com/skywardswordplugin/latest.update";
#endif

const QString SKYWARDSWORD_UPDATE_URL         = "updateUrl";
const QString SKYWARDSWORD_CHECK_ON_START     = QString("checkForUpdatesOnStart");
}
#undef tr
}

#endif // CONSTANTS_HPP
