/**
 * @file main.cpp
 * @brief Contains the entry point for the application as well as the implementation of the UpdateDrawFrame function,
 * responsible for updating and drawing the frame in the application's main loop.
 *
 */
#define RAYGUI_IMPLEMENTATION

#include "jarvis_march.h"
#include "raygui.h"
#include "raylib.h"
#include "timer.h"
#include <iostream>
#include <vector>

#if defined(PLATFORM_WEB)
#include "emscripten/emscripten.h"
// use js to get the body height and width
EM_JS(int, getDocumentBodyWidth, (), { return window.innerWidth; });
EM_JS(int, getDocumentBodyHeight, (), { return window.innerHeight; });
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
/**
 * @brief Enumerates algorithms available in the application.
 *
 * It includes the following algorithms:
 *   - JARVIS_MARCH: Jarvis March Algorithm
 *   - KIRK_PATRICK_SEIDEL: Kirk Patrick Seidel Algorithm
 */
enum Algorithms
{
    JARVIS_MARCH,
    KIRK_PATRICK_SEIDEL
};
/**
 * @brief Represents the custom font used for rendering which is set as default for raygui
 */
Font defaultFont = {0};
/**
 * @brief Represents the currently selected algorithm.
 *
 */
int selectedAlgorithm = 0;
/**
 * @brief Represents the previously selected algorithm.
 *
 * This variable is compared with selectedAlgorithm and change showConvexHull variable only if the selectedAlgorithm is
 * changed.
 */
int previousAlgorithm = selectedAlgorithm;
/**
 * @brief Indicates whether the dropdown menu is open.
 *
 */
int isDropdownOpen = false;
/**
 * @brief Indicates whether to display the convex hull.
 *
 */
bool showConvexHull = false;
/**
 * @brief Height of the toolbar.
 *
 */
const float toolbarHeight = 70;
/**
 * @brief A collection of points (x, y) to be used as test data
 *
 */
std::vector<Vector2> dataPoints;
/**
 * @brief Represents the JarvisMarch object.
 *
 */
JarvisMarch jm(dataPoints);

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void); // Update and draw one frame
Timer frameTimer;

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1270;
    int screenHeight = 720;

#if defined(PLATFORM_WEB)
    // when running on web, set the screen width and height to the body width and height
    screenWidth = getDocumentBodyWidth();
    screenHeight = getDocumentBodyHeight();
#endif

    // dataPoints.push_back({0, 3});
    // dataPoints.push_back({0, 20});
    // dataPoints.push_back({-3, 12});
    // dataPoints.push_back({0, -20});
    // dataPoints.push_back({0, 0});
    // dataPoints.push_back({4, -16});
    // dataPoints.push_back({1.1, 1.2});
    // dataPoints.push_back({-2, 4});
    // dataPoints.push_back({0, 10});
    // dataPoints.push_back({0, -10});

    dataPoints.push_back({4, -2});
    dataPoints.push_back({8, -1});
    dataPoints.push_back({-2, 7});
    dataPoints.push_back({3, 5});
    dataPoints.push_back({-5, 1});
    dataPoints.push_back({-1, 2});
    dataPoints.push_back({-5, -9});
    dataPoints.push_back({-3, -6});
    dataPoints.push_back({7, -4});
    dataPoints.push_back({-6, -1});
    dataPoints.push_back({8, -3});
    dataPoints.push_back({6, 2});
    dataPoints.push_back({3, 4});
    dataPoints.push_back({6, 9});
    dataPoints.push_back({0, -8});
    dataPoints.push_back({-2, -10});
    dataPoints.push_back({4, -5});
    dataPoints.push_back({0, 8});
    dataPoints.push_back({4, 6});
    dataPoints.push_back({-10, -6});

    float centerX = screenWidth / 2.0f;
    float centerY = (screenHeight + toolbarHeight) / 2.0f;
    const float scale = 20.0f;
    for (auto &point : dataPoints)
    {
        point.x = centerX + point.x * scale;
        point.y = centerY + point.y * scale;
    }

    jm = JarvisMarch(dataPoints);

    InitWindow(screenWidth, screenHeight, "Convex Hull");

    // load defaultFont
    defaultFont = LoadFontEx("resources/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Bold.ttf", 30, 0, 0);

    GuiSetFont(defaultFont);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    SetTextureFilter(defaultFont.texture, TEXTURE_FILTER_BILINEAR);

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
bool done = false;
// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    if (frameTimer.isTimerDone())
    {
        if (!done)
        {

            if (jm.orientation(dataPoints[jm.currentPointIndex], dataPoints[jm.comparePointIndex],
                               dataPoints[jm.nextPointIndex]) == 2)
            {
                jm.nextPointIndex = jm.comparePointIndex;
            }

            jm.comparePointIndex = (jm.comparePointIndex + 1) % jm.points.size();

            if (jm.comparePointIndex == 0)
            {
                jm.currentPointIndex = jm.nextPointIndex;
                jm.convexHull.push_back(jm.points[jm.currentPointIndex]);
                jm.nextPointIndex = (jm.nextPointIndex + 1) % jm.points.size();
            }
        }

        if (jm.convexHull.size() > 1 && jm.currentPointIndex == jm.leftMostPointIndex)
        {
            done = true;
            std::cout << jm.convexHull.size() << std::endl;
            // for (auto &p : jm.convexHull)
            // {
            //     std::cout << p.x << " " << p.y << std::endl;
            // }
        }
        frameTimer.resetTimer(0.3);
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // Toolbar
    GuiLine(Rectangle{0, toolbarHeight, static_cast<float>(GetScreenWidth()), 0}, NULL);
    if (GuiDropdownBox(Rectangle{static_cast<float>(GetScreenWidth() - 320), 10, 310, 50},
                       "Jarvis March;Kirk Patrick Seidel", &selectedAlgorithm, isDropdownOpen))
    {
        isDropdownOpen = !isDropdownOpen;
        if (previousAlgorithm != selectedAlgorithm)
        {
            showConvexHull = false;
        }
        previousAlgorithm = selectedAlgorithm;
    }

    if (GuiButton(Rectangle{static_cast<float>(GetScreenWidth() - 640), 10, 310, 50}, "Toggle Convex Hull"))
    {
        showConvexHull = !showConvexHull;
    }

    switch (static_cast<Algorithms>(selectedAlgorithm))
    {
    case JARVIS_MARCH: {
        GuiDrawText("Jarvis March Algorithm", {10, 10, 400, 50}, TEXT_ALIGN_LEFT, BLACK);
        for (size_t i = 0; i < dataPoints.size(); i++)
        {
            DrawCircleV(dataPoints[i], 5, BLACK);
        }

        if (showConvexHull)
        {
            frameTimer.startTimer(0.3);

            int i = jm.leftMostPointIndex;
            DrawCircleV(dataPoints[i], 5, RED);

            i = jm.currentPointIndex;
            DrawCircleV(dataPoints[i], 5, GREEN);

            if (!done)
            {
                DrawCircleV(dataPoints[jm.nextPointIndex], 5, BLUE);
                DrawCircleV(dataPoints[jm.comparePointIndex], 5, PURPLE);
                DrawLineV(dataPoints[jm.currentPointIndex], dataPoints[jm.nextPointIndex], BLACK);
                DrawLineV(dataPoints[jm.currentPointIndex], dataPoints[jm.comparePointIndex], RED);
            }

            for (auto &p : jm.convexHull)
            {
                DrawCircleV(p, 5, MAROON);
            }
            for (size_t i = 0; i < jm.convexHull.size() - 1; i++)
            {
                size_t j = i + 1;
                DrawLineV(jm.convexHull[i], jm.convexHull[j], GREEN);
            }
        }
    }
    break;
    case KIRK_PATRICK_SEIDEL: {
        GuiDrawText("Kirk Patrick Seidel Algorithm", {10, 10, 500, 50}, TEXT_ALIGN_LEFT, BLACK);
    }
    break;
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
}