#ifndef KIRK_PATRICK_SEIDEL_H
#define KIRK_PATRICK_SEIDEL_H

#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

class Kirk
{
  public:
    enum State
    {
        INIT,
        BRIDGE,
        PAIRS,
        UPPER_HULL,
        LOWER_HULL,
        MERGE,
        FINISH
    };

    Kirk(std::vector<Vector2> points);
    ~Kirk();

    void draw();
    void changeState();

  private:
    State currentState = State::INIT;
    float x_mid;
};

#endif // KIRK_PATRICK_SEIDEL_H