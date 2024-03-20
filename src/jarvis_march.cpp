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

    leftMostPointIndex = getLeftMostPointIndex();

    currentPointIndex = leftMostPointIndex;

    if (n != 0)
    {
        nextPointIndex = (leftMostPointIndex + 1) % n;
        convexHull.push_back(points[currentPointIndex]);
    }
}

JarvisMarch::~JarvisMarch()
{
    points.clear();
    convexHull.clear();
}

void JarvisMarch::drawConvexHull()
{
    for (auto &p : convexHull)
    {
        DrawCircle(p.x, p.y, 5, BLUE);
    }

    for (int i = 0; i < convexHull.size() - 1; i++)
    {
        DrawLineEx(convexHull[i], convexHull[i + 1], 2, GREEN);
    }
}

void JarvisMarch::changeState(State s)
{
    currentState = s;
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
        DrawCircleV(points[nextPointIndex], 5, BLUE);
        DrawCircleV(points[comparePointIndex], 5, PURPLE);
        DrawLineV(points[currentPointIndex], points[nextPointIndex], BLACK);
        DrawLineV(points[currentPointIndex], points[comparePointIndex], RED);
    }
    drawConvexHull();
    EndDrawing();
}

void JarvisMarch::update()
{
    switch (currentState)
    {
    case State::INIT:
        if (n < 3)
        {
            convexHull = points;
            currentState = State::FINISHED;
        }

        if (orientation(points[currentPointIndex], points[comparePointIndex], points[nextPointIndex]) ==
            Orientation::COUNTER_CLOCKWISE)
        {
            nextPointIndex = comparePointIndex;
        }

        comparePointIndex = (comparePointIndex + 1);

        if (comparePointIndex == n)
        {
            currentPointIndex = nextPointIndex;
            convexHull.push_back(points[currentPointIndex]);
            nextPointIndex = (currentPointIndex + 1) % n;
            comparePointIndex = 0;
            currentState = State::FINDING_HULL_POINT;
        }

        break;

    case State::FINDING_HULL_POINT:
        if (orientation(points[currentPointIndex], points[comparePointIndex], points[nextPointIndex]) ==
            Orientation::COUNTER_CLOCKWISE)
        {
            nextPointIndex = comparePointIndex;
        }

        comparePointIndex = (comparePointIndex + 1);

        if (comparePointIndex == n)
        {
            currentPointIndex = nextPointIndex;
            convexHull.push_back(points[currentPointIndex]);
            nextPointIndex = (currentPointIndex + 1) % n;
            comparePointIndex = 0;
        }

        if (currentPointIndex == leftMostPointIndex)
        {
            currentState = State::FINISHED;
        }
        break;

    case State::FINISHED:
        break;
    }
}

bool JarvisMarch::isFinished()
{
    return currentState == State::FINISHED;
}