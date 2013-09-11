#include "Common.hpp"

quint64 wiiTime()
{
    time_t sysTime, tzDiff, tzDST;
    struct tm * gmTime;

    time(&sysTime);

    // Account for DST where needed
    gmTime = localtime(&sysTime);
    if(gmTime->tm_isdst == 1)
        tzDST = 3600;
    else
        tzDST = 0;

    // Lazy way to get local time in sec
    gmTime	= gmtime(&sysTime);
    tzDiff = sysTime - mktime(gmTime);

    return (quint64)(TICKS_PER_SECOND * ((sysTime + tzDiff + tzDST) - SECONDS_TO_2000));
}

quint64 toWiiTime(QDateTime time)
{
    time_t sysTime, tzDiff, tzDST;
    struct tm * gmTime;

    sysTime = time.toTime_t();
    // Account for DST where needed
    gmTime = localtime(&sysTime);
    if (!gmTime)
        return 0;
    if(gmTime->tm_isdst == 1)
        tzDST = 3600;
    else
        tzDST = 0;

    // Lazy way to get local time in sec
    gmTime	= gmtime(&sysTime);
    tzDiff = sysTime - mktime(gmTime);

    return (quint64)(TICKS_PER_SECOND * ((sysTime + tzDiff) - SECONDS_TO_2000));
}

QDateTime fromWiiTime(quint64 wiiTime)
{
    QDateTime tmp(QDate(2000, 1, 1));
    tmp = tmp.addSecs(wiiTime / TICKS_PER_SECOND);
    return tmp;
}
