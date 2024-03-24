#ifndef KIRK_PATRICK_SEIDEL_H
#define KIRK_PATRICK_SEIDEL_H

#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

class Kirk
{
  public:
    enum miniState
    {
        LINE,
        PAIRS,
        INTERCEPTS,
        MEDIAN_SLOPE,
        INTERCEPTS_FINAL,
        ADD_TO_CANDIDATES,
        UP_BRIDGE,
        LOW_BRIDGE,
        OVER
    };

    enum State
    {
        INIT,
        UPPER_HULL,
        LOWER_HULL,
        MERGE,
        FINISH
    };

    Kirk(std::vector<Vector2> p);
    ~Kirk();

    void draw();
    void update();
    void previous();
    bool isFinished();
    void drawPrevSteps();
    void drawline(Vector2 p, float slope, Color c);
    static bool compareVector2(Vector2 a, Vector2 b);

  private:
    float median_of_medians(std::vector<float> arr);
    float quick_select(std::vector<float> S, int rank);
    std::vector<Vector2> upper_bridge(std::vector<Vector2> S, float L);
    std::vector<Vector2> upper_hull(std::vector<Vector2> S);
    std::vector<Vector2> lower_hull(std::vector<Vector2> &S);
    std::vector<Vector2> convex_hull(std::vector<Vector2> &S);

    std::vector<Vector2> points;
    std::vector<Vector2> hull;
    std::vector<Vector2> upperHull;

    State currentState = State::INIT;
    miniState currentMiniState = miniState::LINE;
    float x_mid;

    struct Step
    {
        State state;
        miniState type;
        std::vector<Vector2> arr;
        std::vector<Vector2> pairs;
        float x_m = 0, k = 0;
        Vector2 p_k, p_m;
    };

    std::vector<Step> steps;
    int currentStep = 0;

    int currentUpperHullIndex = 0;
    int currentLowerHullIndex = 0;
};

#endif // KIRK_PATRICK_SEIDEL_H