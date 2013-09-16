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
