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
    std::vector<Vector2> points;
    int orientation(Vector2 p, Vector2 q, Vector2 r);

  public:
    JarvisMarch(std::vector<Vector2> points);
    ~JarvisMarch();
    std::vector<Vector2> getConvexHull();
};

#endif