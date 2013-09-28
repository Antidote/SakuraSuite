#ifndef WIIKEYMANAGER_HPP
#define WIIKEYMANAGER_HPP

#include <WiiKeyManagerBase.hpp>
#include <QObject>

class WiiKeyManager : public WiiKeyManagerBase
{
public:
    WiiKeyManager();
    ~WiiKeyManager();
    bool open(const QString& filepath, bool clear = false);
    bool loadKeys();
    void saveKeys();

    bool isOpen() const;
    bool isValid() const;

    QByteArray ngPriv() const;
    void     setNGPriv(const QByteArray& priv);
    quint32    ngId() const;
    void       setNGId(const quint32 id);
    quint32    ngKeyId() const;
    void       setNGKeyId(const quint32 keyId);
    QByteArray ngSig() const;
    void       setNGSig(const QByteArray& sig);
    QByteArray macAddr() const;
    void       setMacAddr(const QByteArray& mac);

private:
    char*   m_ngPriv;
    char*   m_ngSig;
    char*   m_macAddr;
    quint32 m_ngId;
    quint32 m_ngKeyId;
    bool    m_open;
};

#endif // WIIKEYMANAGER_HPP
