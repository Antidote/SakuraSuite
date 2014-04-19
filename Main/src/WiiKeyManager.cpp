#include <WiiKeyManager.hpp>
#include <stdio.h>
#include <QtEndian>
#include <QByteArray>
#include <QSettings>
#include <QStringList>
#include <QDebug>

WiiKeyManager::WiiKeyManager(MainWindow* mainWindow)
    : m_ngPriv(NULL),
      m_ngSig(NULL),
      m_macAddr(NULL),
      m_ngId(0),
      m_ngKeyId(0),
      m_mainWindow(mainWindow)
{
}

WiiKeyManager::~WiiKeyManager()
{
    saveKeys();

    delete[] m_ngPriv;
    delete[] m_ngSig;
    delete[] m_macAddr;
}

bool WiiKeyManager::open(const QString& filepath, bool clear)
{
    if (filepath.isEmpty())
        return false;

    if (clear)
    {
        if (m_ngPriv)
        {
            delete[] m_ngPriv;
            m_ngPriv = NULL;
        }

        if (m_ngSig)
        {
            delete[] m_ngSig;
            m_ngSig = NULL;
        }

        if (m_ngId != 0)
            m_ngId = 0;
        if (m_ngKeyId != 0)
            m_ngKeyId = 0;
    }

    FILE* f = fopen(filepath.toStdString().c_str(), "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        quint32 size = ftell(f);
        fseek(f, 0, SEEK_SET);

        if (size != 0x400)
            return false;

        char buf[0x0C + 1];
        memset(buf, 0, 0x0D);
        fread(buf, 1, 0x0C, f);
        if (strcmp(buf, "BackupMii v1"))
            return false;

        if (!m_ngPriv)
        {
            m_ngPriv = new char[0x1E];
            fseek(f, 0x128, SEEK_SET);
            fread(m_ngPriv, 1, 0x1E, f);
        }
        if (!m_ngSig)
        {
            m_ngSig = new char[0x3C];
            fseek(f, 0x20C, SEEK_SET);
            fread(m_ngSig, 1, 0x3C, f);
        }

        if (m_ngId == 0)
        {
            fseek(f, 0x124, SEEK_SET);
            fread(&m_ngId, 1, 0x04, f);
            m_ngId = qFromBigEndian<quint32>(m_ngId);
        }

        if (m_ngKeyId == 0)
        {
            fseek(f, 0x208, SEEK_SET);
            fread(&m_ngKeyId, 1,  0x04, f);
            m_ngKeyId = qFromBigEndian<quint32>(m_ngKeyId);
        }
        fclose(f);

        qDebug() << "Keys loaded";
        m_open = true;
        return true;
    }
    else
    {
        qWarning() << "File not found" << filepath;
    }
    m_open = false;
    return false;
}

bool WiiKeyManager::loadKeys()
{
    QByteArray tmp;
    bool ok = false;

    QSettings settings;

    if (settings.allKeys().count() <= 0)
        return false;

    tmp = QByteArray::fromHex(settings.value("NGID").toByteArray());
    if (tmp.size() == 4 && !tmp.isEmpty())
    {
        m_ngId = tmp.toHex().toInt(&ok, 16);
        if(ok)
             qDebug() << "Found NGID Successfully:" << tmp.toHex();
        else
        {
            m_ngId = 0;
            qWarning() << "NGID Malformed";
        }
    }
    else
    {
        m_ngId = 0;
        qWarning() << "NGID Not found";
    }

    tmp = QByteArray::fromHex(settings.value("NGKeyID").toByteArray());
    if (tmp.size() == 4 && !tmp.isEmpty())
    {
        m_ngKeyId = tmp.toHex().toInt(&ok, 16);
        if(ok)
            qDebug() << "Found NGKeyID Successfully:" << tmp.toHex();
        else
        {
            m_ngKeyId = 0;
            qWarning() << "NGKeyID Not found";
        }
    }
    else
    {
        m_ngKeyId = 0;
        qWarning() << "NGKeyID Not found";
    }
    tmp = QByteArray::fromHex(settings.value("NGPriv").toByteArray());
    if (tmp.size() == 30 && !tmp.isEmpty())
    {
        setNGPriv(tmp);
        qDebug() << "Found NGPriv Successfully:" << tmp.toHex();
    }
    else
    {
        if (m_ngPriv)
            delete[] m_ngPriv;
        m_ngPriv = NULL;
        qWarning() << "NGPriv Not found";
    }

    tmp = QByteArray::fromHex(settings.value("NGSig").toByteArray());
    if (tmp.size() == 60 && !tmp.isEmpty())
    {
        setNGSig(tmp);
        qDebug() << "Found NGSig Successfully:" << tmp.toHex();
    }
    else
    {
        if (m_ngSig)
            delete[] m_ngSig;
        m_ngSig = NULL;
        qWarning() << "NGSig Not found";
    }
    tmp = QByteArray::fromHex(settings.value("WiiMAC").toByteArray());
    if (tmp.size() == 6 && !tmp.isEmpty())
    {
        setMacAddr(tmp);
        qDebug() << "Found WiiMAC Successfully:" << tmp.toHex();
    }
    else
    {
        if (m_macAddr)
            delete[] m_macAddr;
        m_macAddr = NULL;
        qWarning() << "WiiMAC Not found";
    }

    if (m_ngId > 0 && m_ngKeyId > 0 && m_ngPriv != NULL && m_ngSig != NULL && m_macAddr != NULL)
        return m_open = true;

    return false;
}

void WiiKeyManager::saveKeys()
{
    QByteArray tmp;
    QSettings settings;
    tmp = QByteArray::fromHex(QString::number(m_ngId, 16).toStdString().c_str());
    settings.setValue("NGID", (tmp.size() == 4 ? tmp.toHex() : QByteArray()));
    tmp = QByteArray::fromHex(QString::number(m_ngKeyId, 16).toStdString().c_str());
    settings.setValue("NGKeyID", (tmp.size() == 4 ? tmp.toHex() : QByteArray()));
    settings.setValue("NGSig",  QByteArray(m_ngSig, 0x3C).toHex());
    settings.setValue("NGPriv", QByteArray(m_ngPriv, 0x1E).toHex());
    settings.setValue("WiiMAC", QByteArray(m_macAddr, 0x06).toHex());
}

bool WiiKeyManager::isOpen() const
{
    return m_open;
}

bool WiiKeyManager::isValid() const
{
    return (m_ngId > 0 && m_ngKeyId > 0 && m_ngPriv != NULL && m_ngSig != NULL && m_macAddr != NULL);
}

QByteArray WiiKeyManager::ngPriv() const
{
    return QByteArray(m_ngPriv, 0x1E);
}

void WiiKeyManager::setNGPriv(const QByteArray& priv)
{
    if (m_ngPriv)
        delete[] m_ngPriv;

    m_ngPriv = new char[0x1E];
    memcpy(m_ngPriv, priv.constData(), priv.size());
}

quint32 WiiKeyManager::ngId() const
{
    return m_ngId;
}

void WiiKeyManager::setNGId(const quint32 id)
{
    m_ngId = id;
}

quint32 WiiKeyManager::ngKeyId() const
{
    return m_ngKeyId;
}

void WiiKeyManager::setNGKeyId(const quint32 keyId)
{
    m_ngKeyId = keyId;
}

QByteArray WiiKeyManager::ngSig() const
{
    return QByteArray(m_ngSig, 0x3C);
}

void WiiKeyManager::setNGSig(const QByteArray& sig)
{
    if (m_ngSig)
        delete[] m_ngSig;

    m_ngSig = new char[0x3C];
    memcpy(m_ngSig, sig.constData(), sig.size());
}

QByteArray WiiKeyManager::macAddr() const
{
    return QByteArray(m_macAddr, 0x06);
}

void WiiKeyManager::setMacAddr(const QByteArray& mac)
{
    if (m_macAddr)
        delete[] m_macAddr;

    m_macAddr = new char[0x06];
    memcpy(m_macAddr, mac.constData(), mac.size());
}
