#include "Updater.hpp"
#include <QNetworkAccessManager>
#include <QApplication>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStringList>
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

void Updater::checkForUpdate(QString url, QString currentVersion)
{
    qApp->setOverrideCursor(Qt::WaitCursor);
    m_currentVersion = currentVersion;
    m_manager->get(QNetworkRequest(QUrl(url)));
}

void Updater::onNetworkFinished(QNetworkReply* nr)
{
    QUrl possibleRedirect = nr->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    qDebug() << nr->errorString();

    QUrl redirect = redirectUrl(possibleRedirect, nr->url());
    if (redirect.isValid())
    {
        m_manager->get(QNetworkRequest(redirect));
    }
    else if (nr->error() == QNetworkReply::NoError)
    {
        qApp->restoreOverrideCursor();
        QStringList data = QString(nr->readAll()).split("\n");
        qDebug() << data;

        if (data.filter("version").count() > 0)
        {
            if (data.filter("version").count() > 1)
                emit warning("Found more than one version declaration, using the first");

            QString versionString = data.filter("version")[0];

            if (!versionString.contains("="))
            {
                m_errorString = "Version delcaration is malformed";
                emit error(ParseError);
                return;
            }

            QString serverVersion = versionString.split("=")[1];
            qDebug() << serverVersion;
            qDebug() << m_currentVersion;
            if (!QString::compare(serverVersion, m_currentVersion, Qt::CaseInsensitive))
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

                if (data.filter(platform).count() <= 0)
                {
                    m_errorString = "No update has been declared for this platform";
                    emit error(ParseError);
                    return;
                }

                if (data.filter(platform).count() > 1)
                    emit warning("Found more than one " + platform + " platform declaration, using the first");

                url = data.filter(platform)[0];
                if (url.contains("="))
                    url = url.split("=")[1];
                else
                {
                    m_errorString = "Declaration for this platform (" + platform + ") is malformed";
                    emit error(ParseError);
                }
                m_updateUrl = url;


                if (data.filter(QString("%1-md5").arg(platform)).count() <= 0)
                {
                    m_errorString = "No md5 sum has been declared for this platform";
                    emit error(ParseError);
                    return;
                }

                if (data.filter(platform).count() > 1)
                    emit warning("Found more than one " + platform + "-md5 declaration, using the first");

                QString md5 = data.filter(QString("%1-md5").arg(platform))[0];

                if (!md5.contains("="))
                {
                    m_errorString = "Declaration for this paltform's md5 sum (" + platform + ") is malformed";
                    emit error(ParseError);
                }
                else
                {
                    m_md5Sum = md5.split("=")[1];
                }

                QString changeLog;
                if (data.filter("changelog").count(QString("%1-md5").arg(platform)) > 0)
                {
                    if (data.filter("changelog").count() > 1)
                        emit warning("Found more than one changelog declaration, using the first");

                    QString rawString = data.filter("changelog")[0];
                    if (rawString.contains("="))
                        changeLog = rawString.split("=")[1];
                    else
                    {
                        m_errorString = "Changelog declaration is malformed";
                        emit error(ParseError);
                    }
                }
                else
                {
                    m_changelogUrl = "No changelog specified for this release";
                    emit warning("No changelog specified for this release");
                }

                m_changelogUrl = changeLog;
                emit done();
            }
        }
        else
        {
            m_errorString = "Version has not been declared";
            emit error(ParseError);
        }
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
