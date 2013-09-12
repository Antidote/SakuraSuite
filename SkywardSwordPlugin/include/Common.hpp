// This file is part of WiiKing2 Editor.
//
// WiiKing2 Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Wiiking2 Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WiiKing2 Editor.  If not, see <http://www.gnu.org/licenses/>

#ifndef COMMON_HPP
#define COMMON_HPP

#include <QDateTime>

struct Playtime
{
    int Days;
    int Hours;
    int Minutes;
    int Seconds;

    bool operator ==(const Playtime& val)
    {
        return (val.Days == Days && val.Hours == Hours &&
                val.Minutes == Minutes && val.Seconds == Seconds);
    }
};

struct Vector3
{
    float X;
    float Y;
    float Z;

    Vector3(float x, float y, float z) : X(x), Y(y), Z(z)
    {}
};

const quint64 SECONDS_TO_2000 = 946684800LL;
const quint64 TICKS_PER_SECOND = 60750000LL;

quint64 wiiTime();
quint64 toWiiTime(QDateTime time);
QDateTime fromWiiTime(quint64 wiiTime);

#endif // COMMON_HPP
