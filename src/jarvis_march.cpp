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

        currentStep = 0;
        steps.push_back((struct StepInfo){0, 0, 0, convexHull});
        steps.push_back((struct StepInfo){currentPointIndex, nextPointIndex, candidatePointIndex, convexHull});

        while (currentState != State::FINISHED)
        {
            computeNextStep();
            steps.push_back((struct StepInfo){currentPointIndex, nextPointIndex, candidatePointIndex, convexHull});
        }
    }
}

JarvisMarch::~JarvisMarch()
{
    points.clear();
    convexHull.clear();
    steps.clear();
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
        DrawCircleV(points[steps[currentStep].nextPointIndex], 5, BLUE);
        DrawCircleV(points[steps[currentStep].candidatePointIndex], 5, PURPLE);
        DrawLineV(points[steps[currentStep].currentPointIndex], points[steps[currentStep].nextPointIndex], BLACK);
        DrawLineV(points[steps[currentStep].currentPointIndex], points[steps[currentStep].candidatePointIndex], RED);
    }
    drawConvexHull();
    EndDrawing();
}

void JarvisMarch::computeNextStep()
{
    switch (currentState)
    {
    case State::INIT:
        if (n < 3)
        {
            convexHull = points;
            currentState = State::FINISHED;
        }

        if (orientation(points[currentPointIndex], points[candidatePointIndex], points[nextPointIndex]) ==
            Orientation::COUNTER_CLOCKWISE)
        {
            nextPointIndex = candidatePointIndex;
        }

        candidatePointIndex = (candidatePointIndex + 1);

        if (candidatePointIndex == n)
        {
            currentPointIndex = nextPointIndex;
            convexHull.push_back(points[currentPointIndex]);
            nextPointIndex = (currentPointIndex + 1) % n;
            candidatePointIndex = 0;
            currentState = State::FINDING_HULL_POINT;
        }

        break;

    case State::FINDING_HULL_POINT:
        if (orientation(points[currentPointIndex], points[candidatePointIndex], points[nextPointIndex]) ==
            Orientation::COUNTER_CLOCKWISE)
        {
            nextPointIndex = candidatePointIndex;
        }

        candidatePointIndex = (candidatePointIndex + 1);

        if (candidatePointIndex == n)
        {
            currentPointIndex = nextPointIndex;
            convexHull.push_back(points[currentPointIndex]);
            nextPointIndex = (currentPointIndex + 1) % n;
            candidatePointIndex = 0;
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