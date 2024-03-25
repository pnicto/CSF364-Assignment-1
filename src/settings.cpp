/**
 * @file settings.cpp
 * @brief Contains the implementation of the Settings class.
 *
 */

#include "settings.h"
#include "raygui.h"

Settings::Settings(Vector2 *position, Vector2 *size, Vector2 *contentSz, const char *titleString)
{
    windowPosition = *position;
    windowSize = *size;
    maxWindowSize = *size;
    contentSize = *contentSz;
    title = titleString;
};

Settings::~Settings()
{
}

void Settings::showSettings(bool *showSettings, float toolbarHeight, float bottomBarHeight, float *scale,
                            float *duration, std::string &filePath, bool *isFilePathAdded, float *numberOfPoints,
                            std::vector<Vector2> &fileDataPoints, std::vector<Vector2> &dataPoints)
{
    float statusBarHeight = 24.0f, closeButtonSize = 18.0f;
    if (*showSettings)
    {
        int closeTitleSizeDeltaHalf = (statusBarHeight - closeButtonSize) / 2;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !moving && !resizing)
        {
            Vector2 mousePosition = GetMousePosition();

            Rectangle titleCollisionRect = {windowPosition.x, windowPosition.y,
                                            windowSize.x - (closeButtonSize + closeTitleSizeDeltaHalf),
                                            statusBarHeight};
            Rectangle resizeCollisionRect = {windowPosition.x + windowSize.x - 20.0f,
                                             windowPosition.y + windowSize.y - 20.0f, 20.0f, 20.0f};

            if (CheckCollisionPointRec(mousePosition, titleCollisionRect))
            {
                moving = true;
            }
            else if (!minimized && CheckCollisionPointRec(mousePosition, resizeCollisionRect))
            {
                resizing = true;
            }
        }

        if (moving)
        {
            Vector2 mouseDelta = GetMouseDelta();
            windowPosition.x += mouseDelta.x;
            windowPosition.y += mouseDelta.y;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                moving = false;

                if (windowPosition.x < 0.0f)
                    windowPosition.x = 10.0f;
                else if (windowPosition.x > GetScreenWidth() - windowSize.x)
                    windowPosition.x = GetScreenWidth() - windowSize.x - 10.0f;
                if (windowPosition.y < toolbarHeight)
                    windowPosition.y = toolbarHeight + 10.0f;
                else if (windowPosition.y > GetScreenHeight() - toolbarHeight - bottomBarHeight)
                    windowPosition.y = GetScreenHeight() - bottomBarHeight - statusBarHeight - 10.0f;
            }
        }
        else if (resizing)
        {
            Vector2 mouseDelta = GetMouseDelta();
            windowSize.x += mouseDelta.x;
            windowSize.y += mouseDelta.y;

            if (windowSize.x < 100.0f)
                windowSize.x = 100.0f;
            else if (windowSize.x > GetScreenWidth() - 10.0f)
                windowSize.x = GetScreenWidth() - 10.0f;
            if (windowSize.y < 100.0f)
                windowSize.y = 100.0f;
            else if (windowSize.y > GetScreenHeight() - toolbarHeight - bottomBarHeight - 10.0f)
                windowSize.y = GetScreenHeight() - toolbarHeight - bottomBarHeight - 10.0f;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                resizing = false;
            }
        }

        if (minimized)
        {
            GuiStatusBar(
                (Rectangle){windowPosition.x, windowPosition.y, windowSize.x, statusBarHeight},
                title);

            if (GuiButton((Rectangle){windowPosition.x + windowSize.x - closeButtonSize -
                                          closeTitleSizeDeltaHalf,
                                      windowPosition.y + closeTitleSizeDeltaHalf, closeButtonSize,
                                      closeButtonSize},
                          "#120#"))
            {
                minimized = false;
                windowSize = maxWindowSize;
            }
        }
        else
        {
            minimized =
                GuiWindowBox((Rectangle){windowPosition.x, windowPosition.y, windowSize.x, windowSize.y}, title);
            if (minimized)
            {
                windowSize = {maxWindowSize.x, statusBarHeight};
            }

            Rectangle scissor = {0};
            GuiScrollPanel((Rectangle){windowPosition.x, windowPosition.y + statusBarHeight,
                                       windowSize.x, windowSize.y - statusBarHeight},
                           NULL, (Rectangle){windowPosition.x, windowSize.y, contentSize.x, contentSize.y}, &scroll,
                           &scissor);

            bool requireScissor = windowSize.x < contentSize.x || windowSize.y < contentSize.y;

            if (requireScissor)
            {
                BeginScissorMode(scissor.x, scissor.y, scissor.width, scissor.height);
            }

            drawRandomPointGenerationComponent({20.0f, 50.0f}, {windowPosition.x, windowPosition.y}, {500.0f, 200.0f},
                                               &scroll, toolbarHeight, bottomBarHeight, numberOfPoints, dataPoints);

            drawScaleComponent({20.0f, 50.0f}, {windowPosition.x, windowPosition.y + 150.0f}, {500.0f, 100.0f}, &scroll,
                               scale);

            drawFileInputComponent({20.0f, 50.0f}, {windowPosition.x, windowPosition.y + 250.0f}, {500.0f, 200.0f},
                                   &scroll, isFilePathAdded, filePath, fileDataPoints, dataPoints, toolbarHeight,
                                   bottomBarHeight, scale, scissor);

            drawTimestepComponent({20.0f, 50.0f}, {windowPosition.x, windowPosition.y + 450.0f}, {500.0f, 100.0f},
                                  &scroll, duration);

            if (requireScissor)
            {
                EndScissorMode();
            }

            GuiDrawIcon(71, windowPosition.x + windowSize.x - 20, windowPosition.y + windowSize.y - 20, 1, WHITE);
        }
    }
}

void Settings::drawRandomPointGenerationComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize,
                                                  Vector2 *scroll, float toolbarHeight, float bottomBarHeight,
                                                  float *numberOfPoints, std::vector<Vector2> &dataPoints)
{
    GuiLabel((Rectangle){componentPosition.x + padding.x + (*scroll).x, componentPosition.y + padding.y + (*scroll).y,
                         componentSize.x, (0.125f) * componentSize.y},
             "Number of Points");

    GuiSlider((Rectangle){componentPosition.x + padding.x + (*scroll).x,
                          componentPosition.y + padding.y + ((0.1875f) * (componentSize.y)) + (*scroll).y,
                          componentSize.x, (0.125f) * componentSize.y},
              NULL, TextFormat("%d", (int)(*numberOfPoints)), numberOfPoints, 10.0f, 100.0f);

    if (GuiButton((Rectangle){componentPosition.x + padding.x + (*scroll).x,
                              componentPosition.y + padding.y + ((0.375f) * (componentSize.y)) + (*scroll).y,
                              (0.4f) * componentSize.x, (0.125f) * componentSize.y},
                  "Generate"))
    {
        dataPoints.clear();
        int i = 0;
        while (i < (int)(*numberOfPoints))
        {
            int x = rand() % GetScreenWidth(), y = rand() % GetScreenHeight();
            if (y > (int)toolbarHeight + 10 && y < GetScreenHeight() - bottomBarHeight - 10 && x > 10 &&
                x < GetScreenWidth() - 10)
            {
                dataPoints.push_back({static_cast<float>(x), static_cast<float>(y)});
                i++;
            }
        }
    }
    if (GuiButton((Rectangle){componentPosition.x + padding.x + (*scroll).x + ((0.5f) * componentSize.x),
                              componentPosition.y + padding.y + ((0.375f) * componentSize.y) + (*scroll).y,
                              (0.4f) * componentSize.x, (0.125f) * componentSize.y},
                  "Clear"))
    {
        dataPoints.clear();
    }
}

void Settings::drawScaleComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize, Vector2 *scroll,
                                  float *scale)
{
    GuiLabel((Rectangle){componentPosition.x + padding.x + (*scroll).x, componentPosition.y + padding.y + (*scroll).y,
                         componentSize.x, (0.125f) * componentSize.y},
             "Scale");
    GuiSlider((Rectangle){componentPosition.x + padding.x + (*scroll).x,
                          componentPosition.y + padding.y + (0.375f) * componentSize.y + (*scroll).y, componentSize.x,
                          (0.25f) * componentSize.y},
              NULL, TextFormat("%0.1f", *scale), scale, 0.1f, 50.0f);
}

void Settings::drawFileInputComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize,
                                      Vector2 *scroll, bool *isFilePathAdded, std::string &filePath,
                                      std::vector<Vector2> &fileDataPoints, std::vector<Vector2> &dataPoints,
                                      float toolbarHeight, float bottomBarHeight, float *scale, Rectangle scissor)
{
    if (*isFilePathAdded == 0)
    {
        GuiLabel((Rectangle){componentPosition.x + padding.x + (*scroll).x,
                             componentPosition.y + padding.y + (*scroll).y, componentSize.x,
                             (0.125f) * componentSize.y},
                 "Drop files here!");
        DrawRectangle(componentPosition.x + padding.x + (*scroll).x,
                      componentPosition.y + padding.y + ((0.25f) * componentSize.y) + (*scroll).y, componentSize.x,
                      (0.5f) * componentSize.y, Fade(LIGHTGRAY, 0.3f));

        if (IsFileDropped())
        {
            Vector2 position = GetMousePosition();
            FilePathList droppedFile = LoadDroppedFiles();

            float xCoord = std::max((componentPosition.x + padding.x + (*scroll).x), scissor.x),
                  yCoord = std::max((componentPosition.y + padding.y + ((0.25f) * componentSize.y) + (*scroll).y),
                                    scissor.y),
                  xGap = std::max(0.0f, padding.x + (*scroll).x), yGap = std::min(0.0f, 325.0f + (*scroll).y);

            if (CheckCollisionPointRec(
                    position,
                    (Rectangle){xCoord, yCoord,
                                std::min(scissor.width - (xCoord - scissor.x),
                                         componentSize.x + (padding.x - xGap) + (*scroll).x),
                                std::min(scissor.height - (yCoord - scissor.y), ((0.5f) * componentSize.y) + yGap)}))
            {
                filePath = std::string(droppedFile.paths[0]);
                *isFilePathAdded = 1;

                fileDataPoints.clear();
                std::ifstream istream(filePath);
                float x, y;
                char openParenthesis, closeParenthesis, separator;
                while (istream >> openParenthesis >> x >> separator >> y >> closeParenthesis)
                {
                    fileDataPoints.push_back({x, y});
                }

                computeScale(fileDataPoints, scale, toolbarHeight, bottomBarHeight);
            }

            UnloadDroppedFiles(droppedFile);
        }
    }
    else
    {
        GuiLabel((Rectangle){componentPosition.x + padding.x + (*scroll).x,
                             componentPosition.y + padding.y + (*scroll).y, componentSize.x,
                             (0.125f) * componentSize.y},
                 "Current File");
        DrawRectangle(componentPosition.x + padding.x + (*scroll).x,
                      componentPosition.y + padding.y + ((0.25f) * componentSize.y) + (*scroll).y, componentSize.x,
                      (0.125f) * componentSize.y, Fade(LIGHTGRAY, 0.3f));
        GuiLabel((Rectangle){componentPosition.x + ((1.25f) * padding.x) + (*scroll).x,
                             componentPosition.y + padding.y + ((0.25f) * componentSize.y) + (*scroll).y,
                             componentSize.x, (0.125f) * componentSize.y},
                 filePath.c_str());
        if (GuiButton((Rectangle){componentPosition.x + padding.x + (*scroll).x,
                                  componentPosition.y + padding.y + ((0.5f) * componentSize.y) + (*scroll).y,
                                  (0.4f) * componentSize.x, (0.125f) * componentSize.y},
                      "Draw"))
        {
            if (fileDataPoints.size() > 0)
            {
                dataPoints.clear();
                float smallestX = fileDataPoints[0].x, smallestY = fileDataPoints[0].y;
                for (auto &point : fileDataPoints)
                {
                    smallestX = std::min(smallestX, point.x);
                    smallestY = std::min(smallestY, point.y);
                }
                float centerX = smallestX;
                float centerY = smallestY;

                for (auto &point : fileDataPoints)
                {
                    dataPoints.push_back({(point.x - centerX) * (*scale) + 25.0f,
                                          (point.y - centerY) * (*scale) + toolbarHeight + 25.0f});
                }
            }
        }
        if (GuiButton((Rectangle){componentPosition.x + padding.x + (*scroll).x + ((0.5f) * componentSize.x),
                                  componentPosition.y + padding.y + ((0.5f) * componentSize.y) + (*scroll).y,
                                  (0.4f) * componentSize.x, (0.125f) * componentSize.y},
                      "Clear"))
        {
            *isFilePathAdded = false;
            filePath.clear();
            fileDataPoints.clear();
        }
    }
}

void Settings::drawTimestepComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize, Vector2 *scroll,
                                     float *duration)
{
    GuiLabel((Rectangle){componentPosition.x + padding.x + (*scroll).x, componentPosition.y + padding.y + (*scroll).y,
                         componentSize.x, (0.125f) * componentSize.y},
             "Timestep");
    GuiSlider((Rectangle){componentPosition.x + padding.x + (*scroll).x,
                          componentPosition.y + padding.y + (0.375f) * componentSize.y + (*scroll).y, componentSize.x,
                          (0.25f) * componentSize.y},
              NULL, TextFormat("%0.2f", *duration), duration, 0.01f, 0.50f);
}

void Settings::computeScale(std::vector<Vector2> &fileDataPoints, float *scale, float toolbarHeight,
                            float bottomBarHeight)
{
    if (fileDataPoints.size() > 0)
    {
        float smallestX = fileDataPoints[0].x, largestX = fileDataPoints[0].x, smallestY = fileDataPoints[0].y,
              largestY = fileDataPoints[0].y;
        for (auto &point : fileDataPoints)
        {
            smallestX = std::min(smallestX, point.x);
            largestX = std::max(largestX, point.x);
            smallestY = std::min(smallestY, point.y);
            largestY = std::max(largestY, point.y);
        }

        float computedScale = 50.0f;
        while ((largestX - smallestX) * computedScale > (static_cast<float>(GetScreenWidth()) - 50.0f) ||
               (largestY - smallestY) * computedScale >
                   (static_cast<float>(GetScreenHeight()) - toolbarHeight - bottomBarHeight - 50.0f))
        {
            computedScale -= 0.1f;
        }
        *scale = computedScale;
    }
}

bool Settings::checkPointValidity(Vector2 p, bool *showSettings)
{
    if (!*showSettings)
    {
        return true;
    }
    if (windowPosition.x <= p.x && windowPosition.x + windowSize.x >= p.x && windowPosition.y <= p.y &&
        windowPosition.y + windowSize.y >= p.y)
    {
        return false;
    }
    return true;
}