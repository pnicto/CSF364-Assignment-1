/**
 * @file jarvis_march.cpp
 * @brief Contains the implementation of the JarvisMarch class.
 *
 */
#include "jarvis_march.h"

JarvisMarch::JarvisMarch(std::vector<Vector2> p)
{
    n = p.size();
    points = p;
    if (n != 0)
    {
        computeConvexHull();
    }
}

JarvisMarch::~JarvisMarch()
{
}

void JarvisMarch::drawConvexHull()
{
    for (auto &p : steps[currentStep].convexHull)
    {
        DrawCircle(p.x, p.y, 5, BLUE);
    }
    for (int i = 0; i < steps[currentStep].convexHull.size() - 1; i++)
    {
        DrawLineEx(steps[currentStep].convexHull[i], steps[currentStep].convexHull[i + 1], 2, GREEN);
    }
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
        else if (points[i].x == points[left].x && points[i].y < points[left].y)
        {
            left = i;
        }
    }
    return left;
}

JarvisMarch::Orientation JarvisMarch::orientation(Vector2 p, Vector2 q, Vector2 r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
    {
        return Orientation::COLLINEAR;
    }
    return (val > 0) ? Orientation::CLOCKWISE : Orientation::COUNTER_CLOCKWISE;
}

void JarvisMarch::draw()
{

    BeginDrawing();
    if (!isFinished())
    {
        DrawCircleV(points[steps[currentStep].nextPointIndex], 5, BLUE);
        DrawCircleV(points[steps[currentStep].candidatePointIndex], 5, PURPLE);
        DrawLineV(points[steps[currentStep].currentPointIndex], points[steps[currentStep].nextPointIndex], BLACK);
        DrawLineV(points[steps[currentStep].currentPointIndex], points[steps[currentStep].candidatePointIndex], RED);
    }
    drawConvexHull();
    EndDrawing();
}

void JarvisMarch::next()
{
    if (currentStep < steps.size() - 1)
        currentStep++;
}

void JarvisMarch::previous()
{
    if (currentStep > 0)
        currentStep--;
}

bool JarvisMarch::isFinished()
{
    return currentStep >= steps.size() - 1;
}

void JarvisMarch::computeConvexHull()
{
    if (n < 4)
    {
        convexHull = points;
        return;
    }

    int left = getLeftMostPointIndex();
    int current = left, next;

    do
    {
        convexHull.push_back(points[current]);
        next = (current + 1) % n;

        for (int i = 0; i < n; i++)
        {
            steps.push_back((struct StepInfo){current, next, i, convexHull});
            if (orientation(points[current], points[i], points[next]) == Orientation::COUNTER_CLOCKWISE)
            {
                next = i;
            }
        }

        current = next;
    } while (current != left);
    ;

    convexHull.push_back(convexHull[0]);
    steps.push_back((struct StepInfo){current, next, 0, convexHull});
}