/**
 * @file jarvis_march.h
 * @brief Contains the JarvisMarch class declaration
 *
 */
#ifndef JARVIS_MARCH_H
#define JARVIS_MARCH_H

#include "raylib.h"
#include <iostream>
#include <vector>

class JarvisMarch
{
  private:
    int getLeftMostPointIndex();
    int n;

  public:
    int leftMostPointIndex = 0;
    int currentPointIndex = 0;
    int nextPointIndex = 0;
    int comparePointIndex = 0;
    std::vector<Vector2> points;
    std::vector<Vector2> convexHull;
    JarvisMarch(std::vector<Vector2> points);
    ~JarvisMarch();
    std::vector<Vector2> getConvexHull();
    int orientation(Vector2 p, Vector2 q, Vector2 r);
};

#endif