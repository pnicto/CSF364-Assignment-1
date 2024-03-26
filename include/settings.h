#ifndef SETTINGS_H
#define SETTINGS_H

#include "raylib.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Settings
{
  public:
    Settings(Vector2 *windowPosition, Vector2 *windowSize, Vector2 *contentSize, const char *title);
    ~Settings();
    void showSettings(bool *showSettings, float toolbarHeight, float bottomBarHeight, float *scale, float *duration,
                      std::string &filePath, bool *isFilePathAdded, float *numberOfPoints,
                      std::vector<Vector2> &fileDataPoints, std::vector<Vector2> &dataPoints, float &centerX,
                      float &centerY);
    bool checkPointValidity(Vector2 p, bool *showSettings);

  private:
    Vector2 windowPosition = {0, 0};
    Vector2 windowSize = {0, 0};
    Vector2 maxWindowSize = {0, 0};
    Vector2 contentSize = {0, 0};
    bool minimized = false;
    bool moving = false;
    bool resizing = false;
    Vector2 scroll;
    const char *title = "Title";

    void drawRandomPointGenerationComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize,
                                            Vector2 *scroll, float toolbarHeight, float bottomBarHeight,
                                            float *numberOfPoints, std::vector<Vector2> &dataPoints);
    void drawScaleComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize, Vector2 *scroll,
                            float *scale);
    void drawFileInputComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize, Vector2 *scroll,
                                bool *isFilePathAdded, std::string &filePath, std::vector<Vector2> &fileDataPoints,
                                std::vector<Vector2> &dataPoints, float toolbarHeight, float bottomBarHeight,
                                float *scale, Rectangle scissor, float &centerX, float &centerY);
    void drawTimestepComponent(Vector2 padding, Vector2 componentPosition, Vector2 componentSize, Vector2 *scroll,
                               float *duration);
    void computeScale(std::vector<Vector2> &fileDataPoints, float *scale, float toolbarHeight, float bottomBarHeight);
};

#endif