/**
 * @file settings.h
 * @brief Contains the declaration of the Settings class.
 *
 * This class implements the floating window used to display the settings options.
 *
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include "raylib.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Represents the Settings class, implementing a floating window with various settings options.
 *
 * Usage:
 * 1. Create an instance of the Settings class with the windowPosition, windowSize, contentSize and title.
 * 2. Call the showSettings() method to render the floating window for settings along with the required parameters.
 * 3. Call the checkPointValidity() method to ensure that unwanted points are not added to the canvas when using the
 * settings window.
 *
 */

class Settings
{
  private:
    /**
     * @brief Specifies the current position of the settings window.
     *
     */
    Vector2 windowPosition = {0, 0};
    /**
     * @brief Specifies the current size of the settings window.
     *
     */
    Vector2 windowSize = {0, 0};
    /**
     * @brief Specifies the size of the settings window when maximized.
     *
     */
    Vector2 maxWindowSize = {0, 0};
    /**
     * @brief Specifies the size of the content in the settings window.
     *
     */
    Vector2 contentSize = {0, 0};
    /**
     * @brief Specifies if the settings window is currently minimized.
     *
     */
    bool minimized = false;
    /**
     * @brief Specifies if the settings window is currently being moved.
     *
     */
    bool moving = false;
    /**
     * @brief Specifies if the settings window is currently being resized.
     *
     */
    bool resizing = false;
    /**
     * @brief The object used to keep track of current position after scrolling.
     *
     */
    Vector2 scroll;
    /**
     * @brief Specifies the title of the settings window.
     *
     */
    const char *title = "Settings";

    /**
     * @brief The function used to draw the random point generation component on the screen.
     *
     * @param padding The top and left padding applied to the component.
     * @param componentPosition The current position of the component.
     * @param componentSize The current size of the component.
     * @param scroll The object used to keep track of current position after scrolling.
     * @param toolbarHeight The height of the toolbar.
     * @param bottomBarHeight The height of the bottomBar.
     * @param numberOfPoints The number of points to be drawn during random point generation.
     * @param dataPoints The coordinates of points on which the convex hull is to be computed.
     */
    void drawRandomPointGenerationComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize,
                                            Vector2 *scroll, float toolbarHeight, float bottomBarHeight,
                                            float *numberOfPoints, std::vector<Vector2> &dataPoints);
    /**
     * @brief The function used to draw the scale component on the screen.
     *
     * @param padding The top and left padding applied to the component.
     * @param componentPosition The current position of the component.
     * @param componentSize The current size of the component.
     * @param scroll The object used to keep track of current position after scrolling.
     * @param scale The scale using which the points are drawn on the screen.
     */
    void drawScaleComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize, Vector2 *scroll,
                            float *scale);
    /**
     * @brief The function used to draw the file input component on the screen.
     *
     * @param padding The top and left padding applied to the component.
     * @param componentPosition The current position of the component.
     * @param componentSize The current size of the component.
     * @param scroll The object used to keep track of current position after scrolling.
     * @param isFilePathAdded The variable used to indicate if points from a file have been loaded into memory.
     * @param filePath The filePath of the dropped file.
     * @param fileDataPoints The coordinates of the points obtained from a file.
     * @param dataPoints The coordinates of points on which the convex hull is to be computed.
     * @param toolbarHeight The height of the toolbar.
     * @param bottomBarHeight The height of the bottomBar.
     * @param scale The scale using which the points are drawn on the screen.
     * @param scissor The dimensions of the screen when windowSize is less than contentSize.
     * @param centerX The new X value by which points are shifted to fit on screen.
     * @param centerY The new Y value by which points are shifted to fit on screen.
     */
    void drawFileInputComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize, Vector2 *scroll,
                                bool *isFilePathAdded, std::string &filePath, std::vector<Vector2> &fileDataPoints,
                                std::vector<Vector2> &dataPoints, float toolbarHeight, float bottomBarHeight,
                                float *scale, Rectangle scissor, float &centerX, float &centerY);
    /**
     * @brief The function used to draw the timestep component on the screen.
     *
     * @param padding The top and left padding applied to the component.
     * @param componentPosition The current position of the component.
     * @param componentSize The current size of the component.
     * @param scroll The object used to keep track of current position after scrolling.
     * @param duration The duration after which the timer will be considered done.
     */
    void drawTimestepComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize, Vector2 *scroll,
                               float *duration);
    /**
     * @brief The function used to compute the new scale for the given points from the file, so as to fit them all on
     * screen
     *
     * @param fileDataPoints The coordinates of the points obtained from a file.
     * @param scale The scale using which the points are drawn on the screen.
     * @param toolbarHeight The height of the toolbar.
     * @param bottomBarHeight The height of the bottomBar.
     */
    void computeScale(std::vector<Vector2> &fileDataPoints, float *scale, float toolbarHeight, float bottomBarHeight);

  public:
    /**
     * @brief Construct a new Settings object.
     *
     * @param windowPosition The initial position of the settings window.
     * @param windowSize The initial size of the settings window.
     * @param contentSize The initial size of the content inside the settings window.
     * @param title The title of the settings window.
     */
    Settings(Vector2 *windowPosition, Vector2 *windowSize, Vector2 *contentSize, const char *title);
    /**
     * @brief Destroy the Settings object.
     *
     */
    ~Settings();
    /**
     * @brief Display the settings window on screen.
     *
     * @param showSettings The variable used to indicate if the settings window is to be displayed.
     * @param toolbarHeight The height of the toolbar.
     * @param bottomBarHeight The height of the bottomBar.
     * @param scale The scale using which the points are drawn on the screen.
     * @param duration The duration after which the timer will be considered done.
     * @param filePath The filePath of the dropped file.
     * @param isFilePathAdded The variable used to indicate if points from a file have been loaded into memory.
     * @param numberOfPoints The number of points to be drawn during random point generation.
     * @param fileDataPoints The coordinates of the points obtained from a file.
     * @param dataPoints The coordinates of points on which the convex hull is to be computed.
     * @param centerX The new X value by which points are shifted to fit on screen.
     * @param centerY The new Y value by which points are shifted to fit on screen.
     */
    void showSettings(bool *showSettings, float toolbarHeight, float bottomBarHeight, float *scale, float *duration,
                      std::string &filePath, bool *isFilePathAdded, float *numberOfPoints,
                      std::vector<Vector2> &fileDataPoints, std::vector<Vector2> &dataPoints, float &centerX,
                      float &centerY);
    /**
     * @brief
     *
     * @param p The point whose validity is to be checked.
     * @param showSettings The variable used to indicate if the settings window is currently displayed or not.
     *
     * @return true If the point can be drawn on the screen.
     * @return false If the point should not be drawn on the screen.
     */
    bool checkPointValidity(Vector2 p, bool *showSettings);
};

#endif