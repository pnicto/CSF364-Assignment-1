#ifndef TIMER_H
#define TIMER_H

#include "raylib.h"
#include <iostream>

class Timer
{
  private:
    double startTime;
    double duration;
    bool hasTimerStarted();

  public:
    Timer();
    ~Timer();
    bool isTimerDone();
    void startTimer(double duration);
    double getElapsed();
    void resetTimer(double duration);
};

#endif
