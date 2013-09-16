#include "Updater.hpp"
#include "Constants.hpp"

#include <QNetworkAccessManager>
#include <QApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStringList>
#include <QByteArray>
#include <QDebug>

Updater::Updater(QObject* parent)
    : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), SLOT(onNetworkFinished(QNetworkReply*)));
}

Updater::~Updater()
{
    delete m_manager;
}

QString Updater::updateUrl() const
{
    return m_updateUrl;
}

QString Updater::changelogUrl() const
{
    return m_changelogUrl;
}

QString Updater::errorString() const
{
    return m_errorString;
}

QString Updater::md5Sum() const
{
    return m_md5Sum;
}

void Updater::checkForUpdate(QString url, QString currentVersionString, int currentVersion)
{
    m_currentVersion = currentVersion;
    qApp->setOverrideCursor(Qt::WaitCursor);
    m_currentVersionString = currentVersionString;
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void Updater::parse(QStringList data)
{
    QRegExp regExp(Constants::UPDATER_VERSION_REGEXP, Qt::CaseInsensitive);
    if (data.filter(regExp).count() > 0)
    {
        // Version
        if (data.filter(regExp).count() > 1)
            emit warning("Found more than one version declaration, using the first");

        QString versionString = data.filter(regExp)[0];

        if (!versionString.contains("="))
        {
            m_errorString = "Version delcaration is malformed";
            emit error(ParseError);
            return;
        }

        int serverVersion = versionString.split("=")[1].toInt(NULL, 16);
        if (serverVersion <= m_currentVersion)
        {
            emit noUpdate();
            return;
        }
        else
        {
            QString url;
            QString platform;
#ifdef Q_OS_LINUX
            platform = "unix";
#elif defined(Q_OS_WIN32)
            platform = "win32";
#elif defined(Q_OS_WIN64)
            platform = "win64";
#elif defined(Q_OS_MAC)
            platform = "mac";
#endif
            if (platform.isEmpty())
            {
                m_errorString  = "Unable to determine platform.";
                emit error(NoPlatform);
                return;
            }

            // URL
            regExp.setPattern(Constants::UPDATER_URL_REGEXP.arg(platform));
            if (data.filter(regExp).count() <= 0)
            {
                m_errorString = "No update has been declared for this platform";
                emit error(ParseError);
                return;
            }

            qDebug() << data.filter(regExp).count();
            if (data.filter(regExp).count() > 1)
                emit warning("Found more than one " + platform + " platform declaration, using the first");

            regExp.setPattern(Constants::UPDATER_URL_REGEXP.arg(platform));
            url = data.filter(regExp)[0];
            if (url.contains("="))
                url = url.split("=")[1];
            else
            {
                m_errorString = "Declaration for this platform (" + platform + ") is malformed";
                emit error(ParseError);
            }
            m_updateUrl = url;

            // MD5
            regExp.setPattern(Constants::UPDATER_MD5_REGEXP.arg(platform));
            if (data.filter(regExp).count() <= 0)
            {
                m_errorString = "No md5 sum has been declared for this platform";
                emit error(ParseError);
                return;
            }

            if (data.filter(regExp).count() > 1)
                emit warning("Found more than one md5-" + platform + " declaration, using the first");

            QString md5 = data.filter(regExp)[0];

            if (!md5.contains("="))
            {
                m_errorString = "Declaration for this paltform's md5 sum (" + platform + ") is malformed";
                emit error(ParseError);
            }
            else
            {
                m_md5Sum = md5.split("=")[1];
            }

            // Changelog
            regExp.setPattern(Constants::UPDATER_CHANGELOG_REGEXP);
            if (data.filter(regExp).count() > 0)
            {
                if (data.filter(regExp).count() > 1)
                    emit warning("Found more than one changelog declaration, using the first");

                QString rawString = data.filter(regExp)[0];
                if (rawString.contains("="))
                     m_changelogUrl = rawString.split("=")[1];
                else
                {
                    m_errorString = "Changelog declaration is malformed";
                    emit error(ParseError);
                }
            }
            else
            {
                // the tags are to kill any links that may be in the message
                // Qt parses them out, so having them is no problem
                m_changelogUrl = "</a>No changelog specified for this release<a>";
                emit warning("No changelog specified for this release");
            }

            emit done();
        }
    }
    else
    {
        m_errorString = "Version has not been declared";
        emit error(ParseError);
    }
}

void Updater::onNetworkFinished(QNetworkReply* nr)
{
    QUrl possibleRedirect = nr->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    QUrl redirect = redirectUrl(possibleRedirect, nr->url());

    if (redirect.isValid())
    {
        m_manager->get(QNetworkRequest(redirect));
    }
    else if (nr->error() == QNetworkReply::NoError)
    {
        qApp->restoreOverrideCursor();
        QStringList data = QString(nr->readAll()).split("\n");
        parse(data);
    }
    else
    {
        qApp->restoreOverrideCursor();
        m_errorString = "Unable to contact server " + m_serverUrl;
        emit error(UnableToConnect);
    }
}

QUrl Updater::redirectUrl(const QUrl& possibleRedirect, const QUrl& oldRedirect) const
{
    if (!possibleRedirect.isEmpty() && possibleRedirect != oldRedirect)
        return possibleRedirect;

    return QUrl();

}
