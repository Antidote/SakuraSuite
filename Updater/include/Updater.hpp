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

#ifndef UPDATER_HPP
#define UPDATER_HPP

// KEEP TRANSLATABLE STRINGS OUT OF THIS LIBRARY
#include <QObject>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

class Updater : public QObject
{
    Q_OBJECT
public:
    enum ErrorType
    {
        UnableToConnect,
        NoPlatform,
        ParseError
    };

    Updater(QObject* parent=0);
    ~Updater();

    QString updateUrl() const;
    QString changelogUrl() const;
    QString errorString() const;
    QString md5Sum() const;

public slots:
    void checkForUpdate(QString url, QString currentVersionString, int currentVersion);

signals:
    void noUpdate();
    void done();
    void error(Updater::ErrorType);
    void warning(QString);

private slots:
    /// You may override this in order to fit your needs
    /// but make sure to emit ALL types of errors
    /// so the your plugin/app knows what happened
    virtual void onNetworkFinished(QNetworkReply* nr);
private:
    /// You may override this in order to fit your needs
    virtual void parse(QStringList data);

private:
    QUrl redirectUrl(const QUrl& possibleRedirect, const QUrl& oldRedirect) const;
    QNetworkAccessManager* m_manager;
    QString                m_serverUrl;
    QString                m_updateUrl;
    QString                m_changelogUrl;
    QString                m_currentVersionString;
    int                    m_currentVersion;
    QString                m_errorString;
    QString                m_md5Sum;
};

#endif // UPDATER_HPP
