/**
 * @file settings.cpp
 * @brief Contains the implementation of the Settings class.
 *
 */

#include "settings.h"
#include "raygui.h"

Settings::Settings(Vector2 *position, Vector2 *size, bool *min, bool *mov, bool *resiz, Vector2 *content_sz,
                   const char *titleString)
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
    minimized = *min;
    moving = *mov;
    resizing = *resiz;
    content_size = *content_sz;
    title = titleString;
};

Settings::~Settings()
{
    window_position = {0, 0};
    window_size = {0, 0};
    max_window_size = {0, 0};
    minimized = false;
    moving = false;
    resizing = false;
}

void Settings::showSettings(bool *showSettings, float toolbarHeight, float *scale, float *numberOfPoints,
                            std::vector<Vector2> &dataPoints)
{
    if (*showSettings)
    {
        int close_title_size_delta_half = (RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - RAYGUI_WINDOW_CLOSEBUTTON_SIZE) / 2;

        // window movement and resize input and collision check
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

        // window movement and resize update
        if (moving)
        {
            Vector2 mouse_delta = GetMouseDelta();
            window_position.x += mouse_delta.x;
            window_position.y += mouse_delta.y;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                moving = false;

                // clamp window position keep it inside the application area
                if (window_position.x < 0)
                    window_position.x = 10;
                else if (window_position.x > GetScreenWidth() - window_size.x)
                    window_position.x = GetScreenWidth() - window_size.x - 10;
                if (window_position.y < toolbarHeight)
                    window_position.y = toolbarHeight + 10;
                else if (window_position.y > GetScreenHeight())
                    window_position.y = GetScreenHeight() - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT - 10;
            }
        }
        else if (resizing)
        {
            Vector2 mouse_delta = GetMouseDelta();
            window_size.x += mouse_delta.x;
            window_size.y += mouse_delta.y;

            // clamp window size to an arbitrary minimum value and the window size as the maximum
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

        // window and content drawing with scissor and scroll area
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

            // scissor and draw content within a scroll panel
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

            GuiLabel((Rectangle){window_position.x + 20 + scroll.x, window_position.y + 50 + scroll.y, 250, 25},
                     "Number of Points");
            GuiSlider((Rectangle){window_position.x + 20 + scroll.x, window_position.y + 100 + scroll.y, 300, 25}, NULL,
                      TextFormat("%d", (int)(*numberOfPoints)), numberOfPoints, 10.0f, 100.0f);

            if (GuiButton((Rectangle){window_position.x + 20 + scroll.x, window_position.y + 150 + scroll.y, 150, 25},
                          "Generate"))
            {
                dataPoints.clear();
                int i = 0;
                while (i < (int)(*numberOfPoints))
                {
                    int x = rand() % GetScreenWidth(), y = rand() % GetScreenHeight();
                    if (y > toolbarHeight + 10 && y < GetScreenWidth() - 10 && x > 10 && x < GetScreenWidth() - 10)
                    {
                        dataPoints.push_back({static_cast<float>(x), static_cast<float>(y)});
                        i++;
                    }
                }
            }
            if (GuiButton(
                    (Rectangle){window_position.x + 20 + scroll.x + 160, window_position.y + 150 + scroll.y, 150, 25},
                    "Clear"))
            {
                dataPoints.clear();
            }

            GuiLabel((Rectangle){window_position.x + 20 + scroll.x, window_position.y + 250 + scroll.y, 250, 25},
                     "Scale");
            GuiSlider((Rectangle){window_position.x + 20 + scroll.x, window_position.y + 300 + scroll.y, 300, 25}, NULL,
                      TextFormat("%d", (int)(*scale)), scale, 1.0f, 100.0f);

            GuiLabel((Rectangle){window_position.x + 20 + scroll.x, window_position.y + 350 + scroll.y, 250, 25},
                     "Yet Another Label");

            if (require_scissor)
            {
                EndScissorMode();
            }

            // draw the resize button/icon
            GuiDrawIcon(71, window_position.x + window_size.x - 20, window_position.y + window_size.y - 20, 1, WHITE);
        }
    }
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