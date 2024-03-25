#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stack>
#include <utility>
#include <vector>

class ConvexHullAlgorithm
{
  private:
    std::vector<Vector2> points;
    std::vector<Vector2> convexHull;
    virtual void computeConvexHull() = 0;

  public:
    virtual ~ConvexHullAlgorithm() = default;
    virtual bool isFinished() = 0;
    virtual void draw() = 0;
    virtual void next() = 0;
    virtual void previous() = 0;
    virtual int getNumberOfSteps() = 0;
    virtual int getCurrentStep() = 0;
    virtual void setCurrentStep(int stepIndex) = 0;
    virtual void showLegend(bool *showLegend, Vector2 *windowPosition, Vector2 *windowSize, Vector2 *maxWindowSize,
                            Vector2 *contentSize, Vector2 *scroll, bool *moving, bool *resizing, bool *minimized,
                            float toolbarHeight, float bottomBarHeight, const char *title) = 0;
};

#endif // CONVEX_HULL_H