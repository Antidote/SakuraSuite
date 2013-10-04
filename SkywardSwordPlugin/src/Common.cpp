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

#include "Common.hpp"
#include <ctime>

quint64 wiiTime()
{
    time_t sysTime, tzDiff;
    struct tm * gmTime;

    time(&sysTime);

    // Account for DST where needed
    gmTime = localtime(&sysTime);

    // Lazy way to get local time in sec
    gmTime	= gmtime(&sysTime);
    tzDiff = sysTime - mktime(gmTime);

    return (quint64)(TICKS_PER_SECOND * ((sysTime + tzDiff) - SECONDS_TO_2000));
}

quint64 toWiiTime(QDateTime time)
{
    time_t sysTime, tzDiff;
    struct tm * gmTime;

    sysTime = time.toTime_t();
    // Account for DST where needed
    gmTime = localtime(&sysTime);
    if (!gmTime)
        return 0;

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
