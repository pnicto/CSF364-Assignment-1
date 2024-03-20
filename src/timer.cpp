#include "timer.h"

Timer::Timer()
{
    duration = 0;
    startTime = 0;
}

Timer::~Timer()
{
}

bool Timer::isTimerDone()
{
    if (!hasTimerStarted())
    {
        return false;
    }
    return GetTime() - startTime >= duration;
}

bool Timer::hasTimerStarted()
{
    return startTime != 0;
}

void Timer::startTimer(double duration)
{
    if (!hasTimerStarted())
    {
        startTime = GetTime();
        this->duration = duration;
    }
}

void Timer::resetTimer(double duration)
{
    startTime = GetTime();
    this->duration = duration;
}

double Timer::getElapsed()
{
    return GetTime() - startTime;
}
