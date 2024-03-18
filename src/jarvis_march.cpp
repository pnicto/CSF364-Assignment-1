/**
 * @file jarvis_march.cpp
 * @brief Contains the implementation of the JarvisMarch class.
 *
 */
#include "jarvis_march.h"

JarvisMarch::JarvisMarch(std::vector<Vector2> points)
{
    this->points = points;
}

JarvisMarch::~JarvisMarch()
{
    points.clear();
}

int JarvisMarch::orientation(Vector2 p, Vector2 q, Vector2 r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
    {
        return 0;
    }
    return (val > 0) ? 1 : 2;
}

std::vector<Vector2> JarvisMarch::getConvexHull()
{
    std::vector<Vector2> convexHull;
    int n = points.size();
    if (n < 3)
    {
        return convexHull;
    }

    int l = 0;
    for (int i = 1; i < n; i++)
    {
        if (points[i].x < points[l].x)
        {
            l = i;
        }
    }

    int p = l, q;
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
    } while (p != l);

    return convexHull;
}
