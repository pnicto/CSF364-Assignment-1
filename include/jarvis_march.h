#ifndef JARVIS_MARCH_H
#define JARVIS_MARCH_H

#include "raylib.h"
#include <iostream>
#include <vector>

class JarvisMarch
{
  public:
    JarvisMarch(std::vector<Vector2> points);
    ~JarvisMarch();
    enum State
    {
        INIT,
        FINDING_HULL_POINT,
        FINISHED
    };
    enum Orientation
    {
        COLLINEAR,
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };
    void drawConvexHull();
    void changeState(State s);
    void update();
    void draw();
    std::vector<Vector2> convexHull;
    bool isFinished();

  private:
    std::vector<Vector2> points;
    State currentState = State::INIT;
    int leftMostPointIndex;
    int nextPointIndex;
    int candidatePointIndex = 0;
    int currentPointIndex;
    int getLeftMostPointIndex();
    int n;
    Orientation orientation(Vector2 p, Vector2 q, Vector2 r);
};

#endif