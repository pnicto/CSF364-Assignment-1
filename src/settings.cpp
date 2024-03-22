/**
 * @file settings.cpp
 * @brief Contains the implementation of the Settings class.
 *
 */

#include "settings.h"
#include "raygui.h"

Settings::Settings(Vector2 *position, Vector2 *size, Vector2 *content_sz, const char *titleString)
{
#if !defined(RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT)
#define RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT 24
#endif

#if !defined(RAYGUI_WINDOW_CLOSEBUTTON_SIZE)
#define RAYGUI_WINDOW_CLOSEBUTTON_SIZE 18
#endif

    window_position = *position;
    window_size = *size;
    max_window_size = *size;
    content_size = *content_sz;
    title = titleString;
};

Settings::~Settings()
{
}

void Settings::showSettings(bool *showSettings, float toolbarHeight, float *scale, float *duration,
                            std::string &filePath, bool *isFilePathAdded, float *numberOfPoints,
                            std::vector<Vector2> &fileDataPoints, std::vector<Vector2> &dataPoints)
{
    if (*showSettings)
    {
        int close_title_size_delta_half = (RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - RAYGUI_WINDOW_CLOSEBUTTON_SIZE) / 2;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !moving && !resizing)
        {
            Vector2 mouse_position = GetMousePosition();

            Rectangle title_collision_rect = {window_position.x, window_position.y,
                                              window_size.x -
                                                  (RAYGUI_WINDOW_CLOSEBUTTON_SIZE + close_title_size_delta_half),
                                              RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
            Rectangle resize_collision_rect = {window_position.x + window_size.x - 20,
                                               window_position.y + window_size.y - 20, 20, 20};

            if (CheckCollisionPointRec(mouse_position, title_collision_rect))
            {
                moving = true;
            }
            else if (!minimized && CheckCollisionPointRec(mouse_position, resize_collision_rect))
            {
                resizing = true;
            }
        }

        if (moving)
        {
            Vector2 mouse_delta = GetMouseDelta();
            window_position.x += mouse_delta.x;
            window_position.y += mouse_delta.y;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                moving = false;

                if (window_position.x < 0)
                    window_position.x = 10;
                else if (window_position.x > GetScreenWidth() - window_size.x)
                    window_position.x = GetScreenWidth() - window_size.x - 10;
                if (window_position.y < toolbarHeight)
                    window_position.y = toolbarHeight + 10;
                else if (window_position.y > GetScreenHeight() - toolbarHeight)
                    window_position.y = GetScreenHeight() - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 10;
            }
        }
        else if (resizing)
        {
            Vector2 mouse_delta = GetMouseDelta();
            window_size.x += mouse_delta.x;
            window_size.y += mouse_delta.y;

            if (window_size.x < 100)
                window_size.x = 100;
            else if (window_size.x > GetScreenWidth())
                window_size.x = GetScreenWidth();
            if (window_size.y < 100)
                window_size.y = 100;
            else if (window_size.y > GetScreenHeight())
                window_size.y = GetScreenHeight();

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                resizing = false;
            }
        }

        if (minimized)
        {
            GuiStatusBar(
                (Rectangle){window_position.x, window_position.y, window_size.x, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT},
                title);

            if (GuiButton((Rectangle){window_position.x + window_size.x - RAYGUI_WINDOW_CLOSEBUTTON_SIZE -
                                          close_title_size_delta_half,
                                      window_position.y + close_title_size_delta_half, RAYGUI_WINDOW_CLOSEBUTTON_SIZE,
                                      RAYGUI_WINDOW_CLOSEBUTTON_SIZE},
                          "#120#"))
            {
                minimized = false;
                window_size = max_window_size;
            }
        }
        else
        {
            minimized =
                GuiWindowBox((Rectangle){window_position.x, window_position.y, window_size.x, window_size.y}, title);
            if (minimized)
            {
                window_size = {max_window_size.x, RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT};
            }

            Rectangle scissor = {0};
            GuiScrollPanel((Rectangle){window_position.x, window_position.y + RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT,
                                       window_size.x, window_size.y - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT},
                           NULL, (Rectangle){window_position.x, window_size.y, content_size.x, content_size.y}, &scroll,
                           &scissor);

            bool require_scissor = window_size.x < content_size.x || window_size.y < content_size.y;

            if (require_scissor)
            {
                BeginScissorMode(scissor.x, scissor.y, scissor.width, scissor.height);
            }

            drawRandomPointGenerationComponent({20.0f, 50.0f}, {window_position.x, window_position.y}, {500.0f, 200.0f},
                                               &scroll, toolbarHeight, numberOfPoints, dataPoints);

            drawScaleComponent({20.0f, 50.0f}, {window_position.x, window_position.y + 150.0f}, {500.0f, 100.0f},
                               &scroll, scale);

            drawFileInputComponent({20.0f, 50.0f}, {window_position.x, window_position.y + 250.0f}, {500.0f, 200.0f},
                                   &scroll, isFilePathAdded, filePath, fileDataPoints, dataPoints, toolbarHeight, scale);

            drawTimestepComponent({20.0f, 50.0f}, {window_position.x, window_position.y + 450.0f}, {500.0f, 100.0f},
                                  &scroll, duration);

            if (require_scissor)
            {
                EndScissorMode();
            }

            GuiDrawIcon(71, window_position.x + window_size.x - 20, window_position.y + window_size.y - 20, 1, WHITE);
        }
    }
}

void Settings::drawRandomPointGenerationComponent(Vector2 padding, Vector2 component_position, Vector2 component_size,
                                                  Vector2 *scroll, float toolbarHeight, float *numberOfPoints,
                                                  std::vector<Vector2> &dataPoints)
{
    GuiLabel((Rectangle){component_position.x + padding.x + (*scroll).x, component_position.y + padding.y + (*scroll).y,
                         component_size.x, (0.125f) * component_size.y},
             "Number of Points");

    GuiSlider((Rectangle){component_position.x + padding.x + (*scroll).x,
                          component_position.y + padding.y + ((0.1875f) * (component_size.y)) + (*scroll).y,
                          component_size.x, (0.125f) * component_size.y},
              NULL, TextFormat("%d", (int)(*numberOfPoints)), numberOfPoints, 10.0f, 100.0f);

    if (GuiButton((Rectangle){component_position.x + padding.x + (*scroll).x,
                              component_position.y + padding.y + ((0.375f) * (component_size.y)) + (*scroll).y,
                              (0.4f) * component_size.x, (0.125f) * component_size.y},
                  "Generate"))
    {
        dataPoints.clear();
        int i = 0;
        while (i < (int)(*numberOfPoints))
        {
            int x = rand() % GetScreenWidth(), y = rand() % GetScreenHeight();
            if (y > (int)toolbarHeight + 10 && y < GetScreenWidth() - 10 && x > 10 && x < GetScreenWidth() - 10)
            {
                dataPoints.push_back({static_cast<float>(x), static_cast<float>(y)});
                i++;
            }
        }
    }
    if (GuiButton((Rectangle){component_position.x + padding.x + (*scroll).x + ((0.5f) * component_size.x),
                              component_position.y + padding.y + ((0.375f) * component_size.y) + (*scroll).y,
                              (0.4f) * component_size.x, (0.125f) * component_size.y},
                  "Clear"))
    {
        dataPoints.clear();
    }
}

void Settings::drawScaleComponent(Vector2 padding, Vector2 component_position, Vector2 component_size, Vector2 *scroll,
                                  float *scale)
{
    GuiLabel((Rectangle){component_position.x + padding.x + (*scroll).x, component_position.y + padding.y + (*scroll).y,
                         component_size.x, (0.125f) * component_size.y},
             "Scale");
    GuiSlider((Rectangle){component_position.x + padding.x + (*scroll).x,
                          component_position.y + padding.y + (0.375f) * component_size.y + (*scroll).y, component_size.x,
                          (0.25f) * component_size.y},
              NULL, TextFormat("%0.1f", *scale), scale, 0.1f, 50.0f);
}

void Settings::drawFileInputComponent(Vector2 padding, Vector2 component_position, Vector2 component_size,
                                      Vector2 *scroll, bool *isFilePathAdded, std::string &filePath,
                                      std::vector<Vector2> &fileDataPoints, std::vector<Vector2> &dataPoints,
                                      float toolbarHeight, float *scale)
{
    if (*isFilePathAdded == 0)
    {
        GuiLabel((Rectangle){component_position.x + padding.x + (*scroll).x,
                             component_position.y + padding.y + (*scroll).y, component_size.x,
                             (0.125f) * component_size.y},
                 "Drop files here!");
        DrawRectangle(component_position.x + padding.x + (*scroll).x,
                      component_position.y + padding.y + ((0.25f) * component_size.y) + (*scroll).y, component_size.x,
                      (0.5f) * component_size.y, Fade(LIGHTGRAY, 0.3f));
    }
    else
    {
        GuiLabel((Rectangle){component_position.x + padding.x + (*scroll).x,
                             component_position.y + padding.y + (*scroll).y, component_size.x,
                             (0.125f) * component_size.y},
                 "Current File");
        DrawRectangle(component_position.x + padding.x + (*scroll).x,
                      component_position.y + padding.y + ((0.25f) * component_size.y) + (*scroll).y, component_size.x,
                      (0.125f) * component_size.y, Fade(LIGHTGRAY, 0.3f));
        GuiLabel((Rectangle){component_position.x + ((1.25f) * padding.x) + (*scroll).x,
                             component_position.y + padding.y + ((0.25f) * component_size.y) + (*scroll).y,
                             component_size.x, (0.125f) * component_size.y},
                 filePath.c_str());
        if (GuiButton((Rectangle){component_position.x + padding.x + (*scroll).x,
                                  component_position.y + padding.y + ((0.5f) * component_size.y) + (*scroll).y,
                                  (0.4f) * component_size.x, (0.125f) * component_size.y},
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
        if (GuiButton((Rectangle){component_position.x + padding.x + (*scroll).x + ((0.5f) * component_size.x),
                                  component_position.y + padding.y + ((0.5f) * component_size.y) + (*scroll).y,
                                  (0.4f) * component_size.x, (0.125f) * component_size.y},
                      "Clear"))
        {
            *isFilePathAdded = false;
            filePath.clear();
            fileDataPoints.clear();
        }
    }
}

void Settings::drawTimestepComponent(Vector2 padding, Vector2 component_position, Vector2 component_size,
                                     Vector2 *scroll, float *duration)
{
    GuiLabel((Rectangle){component_position.x + padding.x + (*scroll).x, component_position.y + padding.y + (*scroll).y,
                         component_size.x, (0.125f) * component_size.y},
             "Timestep");
    GuiSlider((Rectangle){component_position.x + padding.x + (*scroll).x,
                          component_position.y + padding.y + (0.375f) * component_size.y + (*scroll).y, component_size.x,
                          (0.25f) * component_size.y},
              NULL, TextFormat("%0.2f", *duration), duration, 0.01f, 0.50f);
}

bool Settings::checkPointValidity(Vector2 p, bool *showSettings)
{
    if (!*showSettings)
    {
        return true;
    }
    if (window_position.x <= p.x && window_position.x + window_size.x >= p.x && window_position.y <= p.y &&
        window_position.y + window_size.y >= p.y)
    {
        return false;
    }
    return true;
}

void Settings::computeScale(std::vector<Vector2> &fileDataPoints, float *scale, float toolbarHeight)
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
               (largestY - smallestY) * computedScale > (static_cast<float>(GetScreenHeight()) - toolbarHeight - 50.0f))
        {
            computedScale -= 0.1f;
        }
        *scale = computedScale;
    }
}