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
#include "settings.h"
#include "timer.h"
#include <fstream>
#include <iostream>
#include <string>
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
 * @brief Indicates whether to display the settings modal.
 *
 */
bool showSettings = false;
/**
 * @brief Specifies the position for the settings window
 *
 */
Vector2 window_position = {10, 80};
/**
 * @brief Specifies the size of the settings window
 *
 */
Vector2 window_size = {400, 400};
/**
 * @brief Specifies the size of the content to be displayed in the settings window
 *
 */
Vector2 content_size = {600, 600};
/**
 * @brief Specifies the scale for drawing points
 *
 */
float scale = 20.0f;
/**
 * @brief Specifies the file path from which points are loaded
 *
 */
std::string filePath;
/**
 * @brief Specifies where the file path has been added or not
 *
 */
bool isFilePathAdded = 0;
/**
 * @brief Specifies the number of points to be randomly generated
 *
 */
float numberOfPoints = 10.0f;
/**
 * @brief Specifies the duration for the timer
 *
 */
float duration = 0.01f;
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
 * @brief A collection of points (x, y) obtained from a file before scaling
 *
 */
std::vector<Vector2> fileDataPoints;
/**
 * @brief Represents the JarvisMarch object.
 *
 */
JarvisMarch jm(dataPoints);
/**
 * @brief Represents the Settings object.
 *
 */
Settings settings(&window_position, &window_size, &content_size, "Settings");

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

    float centerX = screenWidth / 2.0f;
    float centerY = (screenHeight + toolbarHeight) / 2.0f;
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
// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    if (frameTimer.isTimerDone() && !jm.isFinished())
    {
        jm.update();
        frameTimer.resetTimer(duration);
    }

    if (!showConvexHull)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetMousePosition();
            if (mousePos.y > toolbarHeight && settings.checkPointValidity(mousePos, &showSettings))
            {
                dataPoints.push_back(mousePos);
            }
        }
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if (dataPoints.size() > 0)
            {
                dataPoints.pop_back();
            }
        }
        jm = JarvisMarch(dataPoints);
    }

    if (IsFileDropped())
    {
        Vector2 position = GetMousePosition();
        FilePathList droppedFile = LoadDroppedFiles();

        if (window_position.x + 20 <= position.x && window_position.x + 20 + 500 >= position.x &&
            window_position.y + 330 <= position.y && window_position.y + 330 + 100 >= position.y)
        {
            filePath = std::string(droppedFile.paths[0]);
            isFilePathAdded = 1;

            fileDataPoints.clear();
            std::ifstream istream(filePath);
            float x, y;
            char openParenthesis, closeParenthesis, separator;
            while (istream >> openParenthesis >> x >> separator >> y >> closeParenthesis)
            {
                fileDataPoints.push_back({x, y});
            }

            settings.computeScale(fileDataPoints, &scale, toolbarHeight);
        }

        UnloadDroppedFiles(droppedFile);
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
        showSettings = false;
    }

    if (GuiButton(Rectangle{static_cast<float>(GetScreenWidth() - 840), 10, 190, 50}, "Settings"))
    {
        if (showConvexHull)
        {
            showConvexHull = !showConvexHull;
        }
        showSettings = !showSettings;
    }

    settings.showSettings(&showSettings, toolbarHeight, &scale, &duration, filePath, &isFilePathAdded, &numberOfPoints,
                          fileDataPoints, dataPoints);

    switch (static_cast<Algorithms>(selectedAlgorithm))
    {
    case JARVIS_MARCH: {
        GuiDrawText("Jarvis March Algorithm", {10, 10, 400, 50}, TEXT_ALIGN_LEFT, BLACK);
        for (size_t i = 0; i < dataPoints.size(); i++)
        {
            if (settings.checkPointValidity(dataPoints[i], &showSettings))
                DrawCircleV(dataPoints[i], 5, BLACK);
        }

        if (showConvexHull)
        {
            if (!jm.isFinished())
            {

                frameTimer.startTimer(0.5);
            }
            else
            {
                frameTimer.stopTimer();
            }
            jm.draw();
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