/**
 * @file timer.h
 * @brief Contains the declaration of the Timer class.
 *
 */
#ifndef TIMER_H
#define TIMER_H

#include "raylib.h"
#include <iostream>

/**
 * @brief Utility class to check if a certain amount of time has passed.
 *
 */
class Timer
{
  private:
    /**
     * @brief The time at which the timer started.
     *
     */
    double startTime;
    /**
     * @brief The duration after which the timer will be considered done.
     *
     */
    double duration;
    /**
     * @brief Checks if the timer has started.
     *
     * @return true if the timer has started
     * @return false if the timer has not started
     */
    bool hasTimerStarted();

  public:
    /**
     * @brief Construct a new Timer object
     *
     */
    Timer();
    /**
     * @brief Destroy the Timer object
     *
     */
    ~Timer();
    /**
     * @brief Checks if the timer has finished.
     *
     * @return true if the timer has finished
     * @return false if the timer has not finished
     */
    bool isTimerDone();
    /**
     * @brief Starts the timer.
     *
     * @param duration the duration after which the timer will be considered done
     */
    void startTimer(double duration);
    /**
     * @brief Gets the time elapsed since the timer started.
     *
     * @return double the time elapsed since the timer started
     */
    double getElapsed();
    /**
     * @brief Resets the timer.
     *
     * @param duration the duration after which the timer will be considered done
     */
    void resetTimer(double duration);
    /**
     * @brief Stops the timer.
     *
     */
    void stopTimer();
};

#endif
