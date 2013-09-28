#ifndef WIIKEYMANAGERBASE_HPP
#define WIIKEYMANAGERBASE_HPP

#include <QByteArray>

class WiiKeyManagerBase
{
public:
    virtual ~WiiKeyManagerBase() {}
    virtual bool       isValid()const=0;
    virtual QByteArray ngPriv() const=0;
    virtual quint32    ngId()   const=0;
    virtual quint32    ngKeyId()const=0;
    virtual QByteArray ngSig()  const=0;
    virtual QByteArray macAddr()const=0;
};

#endif // WIIKEYMANAGERBASE_HPP
