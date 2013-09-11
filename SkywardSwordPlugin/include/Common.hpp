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
