/**
 * @file main.cpp
 * @brief Contains the entry point for the application as well as the implementation of the UpdateDrawFrame function,
 * responsible for updating and drawing the frame in the application's main loop.
 *
 */
#define RAYGUI_IMPLEMENTATION

#include "jarvis_march.h"
#include "kirk_patrick_seidel.h"
#include "raygui.h"
#include "raylib.h"
#include "settings.h"
#include "timer.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#if defined(PLATFORM_WEB)
#include "emscripten/emscripten.h"
// use js to get the body height and width
/**
 * @brief EM JS object that gets the browser window's inner width.
 *
 */
EM_JS(int, getDocumentBodyWidth, (), { return window.innerWidth; });
/**
 * @brief EM JS object that gets the browser window's inner height.
 *
 */
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
bool isDropdownOpen = false;
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
 * @brief Indicates whether to display the legend modal.
 *
 */
bool showLegend = false;
/**
 * @brief Specifies the position for the settings window
 *
 */
Vector2 settingsWindowPosition = {10, 80};
/**
 * @brief Specifies the size of the settings window
 *
 */
Vector2 settingsWindowSize = {400, 400};
/**
 * @brief Specifies the size of the content to be displayed in the settings window
 *
 */
Vector2 settingsContentSize = {600, 600};
/**
 * @brief Specifies the position for the legend window
 *
 */
Vector2 legendWindowPosition = {10, 80};
/**
 * @brief Specifies the size of the legend window
 *
 */
Vector2 legendWindowSize = {450, 200};
/**
 * @brief Specifies the maximum size of the legend window
 *
 */
Vector2 legendWindowMaximumSize = {450, 200};
/**
 * @brief Specifies the size of the content to be displayed in the legend window
 *
 */
Vector2 legendContentSize = {400, 250};
/**
 * @brief Scroll object associated with the legend window
 *
 */
Vector2 legendScroll = {0, 0};
/**
 * @brief Specifies whether the legend window is currently being moved
 *
 */
bool moving = false;
/**
 * @brief Specifies whether the legend window is currently being resized
 *
 */
bool resizing = false;
/**
 * @brief Specifies whether the legend window is currently minimized
 *
 */
bool minimized = false;
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
const float toolbarHeight = 50;
/**
 * @brief A collection of points (x, y) to be used as test data
 *
 */
std::vector<Vector2> dataPoints;
/**
 * @brief Indicates whether to visualize the algorithm one step at a time or play the full thing.
 *
 */
bool visualizeStepByStep = true;
/**
 * @brief Helps check whether the step number has been changed manually (via the slider), in which case
 * visualizeStepByStep made true.
 *
 */
float lastStep = 0;
/**
 * @brief A collection of points (x, y) obtained from a file before scaling
 *
 */
std::vector<Vector2> fileDataPoints;
/**
 * @brief Represents the ConvexHullAlgorithm object.
 *
 */
std::unique_ptr<ConvexHullAlgorithm> ch;
/**
 * @brief Represents the Settings object.
 *
 */
Settings settings(&settingsWindowPosition, &settingsWindowSize, &settingsContentSize, "Settings");
/**
 * @brief Height of the bottom bar.
 *
 */
const float bottomBarHeight = 60;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
/**
 * @brief Updates and draws the frame.
 *
 */
static void UpdateDrawFrame(void);
/**
 * @brief Represents a timer for controlling frame updates.
 *
 */
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

    InitWindow(screenWidth, screenHeight, "Convex Hull");

    // load defaultFont
    defaultFont = LoadFontEx("resources/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Bold.ttf", 30, 0, 0);

    GuiSetFont(defaultFont);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
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
    if (frameTimer.isTimerDone() && !ch->isFinished() && !visualizeStepByStep)
    {
        ch->next();
        frameTimer.resetTimer(duration);
        lastStep = ch->getCurrentStep();
    }

    if (!showConvexHull && !isDropdownOpen)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePos = GetMousePosition();
            if (mousePos.y > toolbarHeight && mousePos.y < GetScreenHeight() - bottomBarHeight &&
                settings.checkPointValidity(mousePos, &showSettings))
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
        selectedAlgorithm == JARVIS_MARCH ? ch = std::make_unique<JarvisMarch>(dataPoints)
                                          : ch = std::make_unique<KirkpatrickSeidel>(dataPoints);
    }

    if (showConvexHull)
    {
        if (IsKeyPressed(KEY_L))
        {
            showLegend = !showLegend;
        }
    }

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    switch (static_cast<Algorithms>(selectedAlgorithm))
    {
    case JARVIS_MARCH: {
        GuiDrawText("Jarvis March Algorithm", {10, 10, 250, 30}, TEXT_ALIGN_LEFT, BLACK);
    }
    break;
    case KIRK_PATRICK_SEIDEL: {
        GuiDrawText("Kirkpatrick-Seidel Algorithm", {10, 10, 300, 30}, TEXT_ALIGN_LEFT, BLACK);
    }
    break;
    }

    for (size_t i = 0; i < dataPoints.size(); i++)
    {
        DrawCircleV(dataPoints[i], 5, BLACK);
    }

    if (showConvexHull)
    {
        if (!ch->isFinished())
        {

            frameTimer.startTimer(0.5);
        }
        else
        {
            frameTimer.stopTimer();
        }
        ch->draw();
    }

    // Toolbar
    GuiLine(Rectangle{0, toolbarHeight, static_cast<float>(GetScreenWidth()), 0}, NULL);
    if (GuiDropdownBox(Rectangle{static_cast<float>(GetScreenWidth() - 260), 10, 250, 30},
                       "Jarvis March;Kirkpatrick-Seidel", &selectedAlgorithm, isDropdownOpen))
    {
        isDropdownOpen = !isDropdownOpen;
        if (previousAlgorithm != selectedAlgorithm)
        {
            showConvexHull = false;
            visualizeStepByStep = true;
            frameTimer.stopTimer();
        }
        previousAlgorithm = selectedAlgorithm;
    }

    // Draw bottom bar
    GuiLine(Rectangle{0, GetScreenHeight() - bottomBarHeight, static_cast<float>(GetScreenWidth()), 0}, NULL);

    // disable the GuiButton when there are no points
    if (dataPoints.size() == 0)
        GuiDisable();
    if (GuiButton(Rectangle{static_cast<float>(GetScreenWidth() - 480), 10, 210, 30}, "Toggle Convex Hull"))
    {
        showConvexHull = !showConvexHull;
        showSettings = false;
        showLegend = false;
    }
    // enable the remaining GUI
    if (dataPoints.size() == 0)
        GuiEnable();

    if (GuiButton(Rectangle{static_cast<float>(GetScreenWidth() - 590), 10, 100, 30}, "Settings"))
    {
        if (showConvexHull)
        {
            showConvexHull = !showConvexHull;
        }
        showSettings = !showSettings;
    }

    if (showConvexHull)
    {
        if (GuiButton(Rectangle{static_cast<float>(GetScreenWidth() - 810), 10, 210, 30},
                      (visualizeStepByStep) ? "Play automatically" : "Play step by step"))
        {
            visualizeStepByStep = !visualizeStepByStep;
        }
    }

    if (showConvexHull)
    {
        int maxSteps;
        float currentStep;

        float h = GetScreenHeight() - bottomBarHeight + 15;

        maxSteps = ch->getNumberOfSteps() - 1;
        currentStep = ch->getCurrentStep();
        if (currentStep != lastStep)
        {
            visualizeStepByStep = true;
        }

        if (currentStep == 0)
            GuiDisable();
        if (GuiButton(Rectangle{70, h, 70, 30}, "Prev"))
        {
            ch->previous();
            currentStep = ch->getCurrentStep();
            visualizeStepByStep = true;
        }
        GuiEnable();

        if (currentStep >= maxSteps)
            GuiDisable();
        if (GuiButton(Rectangle{GetScreenWidth() - 150.0f + 10.0f, h, 70, 30}, "Next"))
        {
            ch->next();
            currentStep = ch->getCurrentStep();
            visualizeStepByStep = true;
        }
        GuiEnable();

        GuiSlider(Rectangle{150, h, GetScreenWidth() - 300.0f, 30}, NULL, NULL, &(currentStep), 0, maxSteps);
        GuiDrawText(TextFormat("%d/%d", static_cast<int>(currentStep + 1), maxSteps + 1),
                    {150 + (GetScreenWidth() - 450.0f) / 2, h, 150, 30}, TEXT_ALIGN_LEFT, BLACK);
        ch->setCurrentStep(currentStep);

        ch->showLegend(&showLegend, &legendWindowPosition, &legendWindowSize, &legendWindowMaximumSize,
                       &legendContentSize, &legendScroll, &moving, &resizing, &minimized, toolbarHeight,
                       bottomBarHeight, "Legend");
        lastStep = currentStep;
    }
    settings.showSettings(&showSettings, toolbarHeight, bottomBarHeight, &scale, &duration, filePath, &isFilePathAdded,
                          &numberOfPoints, fileDataPoints, dataPoints);
    EndDrawing();
    //----------------------------------------------------------------------------------
}