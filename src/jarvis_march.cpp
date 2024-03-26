/**
 * @file jarvis_march.cpp
 * @brief Contains the implementation of the JarvisMarch class.
 *
 */
#include "jarvis_march.h"
#include "raygui.h"

JarvisMarch::JarvisMarch(std::vector<Vector2> p)
{
    n = p.size();
    points = p;
    currentStep = 0;
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

    // BeginDrawing();
    if (!isFinished())
    {
        DrawCircleV(points[steps[currentStep].nextPointIndex], 5, ORANGE);
        DrawCircleV(points[steps[currentStep].candidatePointIndex], 5, PURPLE);
        DrawLineV(points[steps[currentStep].currentPointIndex], points[steps[currentStep].nextPointIndex], BLACK);
        DrawLineV(points[steps[currentStep].currentPointIndex], points[steps[currentStep].candidatePointIndex], RED);
    }
    drawConvexHull();
    // EndDrawing();
}

void JarvisMarch::next()
{
    if (currentStep < steps.size() - 1)
    {
        currentStep++;
    }
}

void JarvisMarch::previous()
{
    if (currentStep > 0)
        currentStep--;
}

int JarvisMarch::getNumberOfSteps()
{

    return steps.size();
}

int JarvisMarch::getCurrentStep()
{
    return currentStep;
}

bool JarvisMarch::isFinished()
{
    return currentStep >= steps.size() - 1;
}

void JarvisMarch::computeConvexHull()
{
    if (n < 3)
    {
        convexHull = points;
        steps.push_back((struct StepInfo){0, 0, 0, convexHull});
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

void JarvisMarch::setCurrentStep(int step)
{
    currentStep = step;
}

std::vector<Vector2> JarvisMarch::exportHull()
{
    return convexHull;
}

void JarvisMarch::showLegend(bool *showLegend, Vector2 *windowPosition, Vector2 *windowSize, Vector2 *maxWindowSize,
                             Vector2 *contentSize, Vector2 *scroll, bool *moving, bool *resizing, bool *minimized,
                             float toolbarHeight, float bottomBarHeight, const char *title)
{
    float statusBarHeight = 24.0f, closeButtonSize = 18.0f;
    if (*showLegend)
    {
        int closeTitleSizeDeltaHalf = (statusBarHeight - closeButtonSize) / 2;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !(*moving) && !(*resizing))
        {
            Vector2 mousePosition = GetMousePosition();

            Rectangle titleCollisionRect = {(*windowPosition).x, (*windowPosition).y,
                                            (*windowSize).x - (closeButtonSize + closeTitleSizeDeltaHalf),
                                            statusBarHeight};
            Rectangle resizeCollisionRect = {(*windowPosition).x + (*windowSize).x - 20.0f,
                                             (*windowPosition).y + (*windowSize).y - 20.0f, 20.0f, 20.0f};

            if (CheckCollisionPointRec(mousePosition, titleCollisionRect))
            {
                (*moving) = true;
            }
            else if (!(*minimized) && CheckCollisionPointRec(mousePosition, resizeCollisionRect))
            {
                (*resizing) = true;
            }
        }

        if ((*moving))
        {
            Vector2 mouseDelta = GetMouseDelta();
            (*windowPosition).x += mouseDelta.x;
            (*windowPosition).y += mouseDelta.y;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                (*moving) = false;

                if ((*windowPosition).x < 0.0f)
                    (*windowPosition).x = 10.0f;
                else if ((*windowPosition).x > GetScreenWidth() - (*windowSize).x)
                    (*windowPosition).x = GetScreenWidth() - (*windowSize).x - 10.0f;
                if ((*windowPosition).y < toolbarHeight)
                    (*windowPosition).y = toolbarHeight + 10;
                else if ((*windowPosition).y > GetScreenHeight() - toolbarHeight - bottomBarHeight)
                    (*windowPosition).y = GetScreenHeight() - bottomBarHeight - statusBarHeight - 10.0f;
            }
        }
        else if ((*resizing))
        {
            Vector2 mouseDelta = GetMouseDelta();
            (*windowSize).x += mouseDelta.x;
            (*windowSize).y += mouseDelta.y;

            if ((*windowSize).x < 100.0f)
                (*windowSize).x = 100.0f;
            else if ((*windowSize).x > GetScreenWidth() - 10.0f)
                (*windowSize).x = GetScreenWidth() - 10.0f;
            if ((*windowSize).y < 100.0f)
                (*windowSize).y = 100.0f;
            else if ((*windowSize).y > GetScreenHeight() - toolbarHeight - bottomBarHeight - 10.0f)
                (*windowSize).y = GetScreenHeight() - toolbarHeight - bottomBarHeight - 10.0f;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                (*resizing) = false;
            }
        }

        if ((*minimized))
        {
            GuiStatusBar((Rectangle){(*windowPosition).x, (*windowPosition).y, (*windowSize).x, statusBarHeight},
                         title);

            if (GuiButton((Rectangle){(*windowPosition).x + (*windowSize).x - closeButtonSize - closeTitleSizeDeltaHalf,
                                      (*windowPosition).y + closeTitleSizeDeltaHalf, closeButtonSize, closeButtonSize},
                          "#120#"))
            {
                (*minimized) = false;
                (*windowSize) = (*maxWindowSize);
            }
        }
        else
        {
            (*minimized) = GuiWindowBox(
                (Rectangle){(*windowPosition).x, (*windowPosition).y, (*windowSize).x, (*windowSize).y}, title);
            if ((*minimized))
            {
                (*windowSize) = {(*maxWindowSize).x, statusBarHeight};
            }

            Rectangle scissor = {0};
            GuiScrollPanel((Rectangle){(*windowPosition).x, (*windowPosition).y + statusBarHeight, (*windowSize).x,
                                       (*windowSize).y - statusBarHeight},
                           NULL, (Rectangle){(*windowPosition).x, (*windowSize).y, (*contentSize).x, (*contentSize).y},
                           scroll, &scissor);

            bool requireScissor = (*windowSize).x < (*contentSize).x || (*windowSize).y < (*contentSize).y;

            if (requireScissor)
            {
                BeginScissorMode(scissor.x, scissor.y, scissor.width, scissor.height);
            }

            DrawCircleV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 50.0f + (*scroll).y}, 5,
                        BLUE);
            GuiLabel(
                {(*windowPosition).x + 30.0f + (*scroll).x, (*windowPosition).y + 35.0f + (*scroll).y, 300.0f, 30.0f},
                " - Convex Hull Point");

            DrawCircleV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 75.0f + (*scroll).y}, 5,
                        ORANGE);
            GuiLabel(
                {(*windowPosition).x + 30.0f + (*scroll).x, (*windowPosition).y + 60.0f + (*scroll).y, 300.0f, 30.0f},
                " - Candidate Point");

            DrawCircleV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 100.0f + (*scroll).y}, 5,
                        PURPLE);
            GuiLabel(
                {(*windowPosition).x + 30.0f + (*scroll).x, (*windowPosition).y + 85.0f + (*scroll).y, 300.0f, 30.0f},
                " - Current Check Point");

            DrawLineV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 125.0f + (*scroll).y},
                      {(*windowPosition).x + 80.0f + (*scroll).x, (*windowPosition).y + 125.0f + (*scroll).y}, GREEN);
            GuiLabel(
                {(*windowPosition).x + 90.0f + (*scroll).x, (*windowPosition).y + 110.0f + (*scroll).y, 300.0f, 30.0f},
                " - Convex Hull Line");

            DrawLineV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 150.0f + (*scroll).y},
                      {(*windowPosition).x + 80.0f + (*scroll).x, (*windowPosition).y + 150.0f + (*scroll).y}, BLACK);
            GuiLabel(
                {(*windowPosition).x + 90.0f + (*scroll).x, (*windowPosition).y + 135.0f + (*scroll).y, 300.0f, 30.0f},
                " - Candidate Hull Line");

            DrawLineV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 175.0f + (*scroll).y},
                      {(*windowPosition).x + 80.0f + (*scroll).x, (*windowPosition).y + 175.0f + (*scroll).y}, RED);
            GuiLabel(
                {(*windowPosition).x + 90.0f + (*scroll).x, (*windowPosition).y + 160.0f + (*scroll).y, 300.0f, 30.0f},
                " - Current Check Line");

            if (requireScissor)
            {
                EndScissorMode();
            }

            GuiDrawIcon(71, (*windowPosition).x + (*windowSize).x - 20, (*windowPosition).y + (*windowSize).y - 20, 1,
                        WHITE);
        }
    }
}