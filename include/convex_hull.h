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
};

#endif // CONVEX_HULL_H