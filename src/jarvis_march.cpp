/**
 * @file jarvis_march.cpp
 * @brief Contains the implementation of the JarvisMarch class.
 *
 */
#include "jarvis_march.h"

JarvisMarch::JarvisMarch(std::vector<Vector2> p)
{
    points = p;
    n = points.size();
    leftMostPointIndex = getLeftMostPointIndex();
    currentPointIndex = leftMostPointIndex;

    // TODO: Uh deal with this later
    if (n != 0)
    {
        nextPointIndex = (leftMostPointIndex + 1) % n;
        comparePointIndex = (leftMostPointIndex + 2) % n;
    }
}

JarvisMarch::~JarvisMarch()
{
    points.clear();
    convexHull.clear();
}

int JarvisMarch::orientation(Vector2 p, Vector2 q, Vector2 r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
    {
        return 0;
    }
    // clock or counterclock wise
    return (val > 0) ? 1 : 2;
}

int JarvisMarch::getLeftMostPointIndex()
{
    int left = 0;
    for (int i = 0; i < n; i++)
    {
        if (points[i].x < points[left].x)
        {
            left = i;
        }
    }
    convexHull.push_back(points[left]);
    return left;
}

std::vector<Vector2> JarvisMarch::getConvexHull()
{
    std::vector<Vector2> convexHull;
    int n = points.size();
    if (n < 3)
    {
        return convexHull;
    }

    int p = leftMostPointIndex, q;
    do
    {
        convexHull.push_back(points[p]);
        q = (p + 1) % n;
        for (int i = 0; i < n; i++)
        {
            if (orientation(points[p], points[i], points[q]) == 2)
            {
                q = i;
            }
        }
        p = q;
    } while (p != leftMostPointIndex);

    return convexHull;
}
