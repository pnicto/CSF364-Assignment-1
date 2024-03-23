#ifndef SETTINGS_H
#define SETTINGS_H

#include "raylib.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Settings
{
  public:
    Settings(Vector2 *window_position, Vector2 *window_size, Vector2 *content_size, const char *title);
    ~Settings();
    void showSettings(bool *showSettings, float toolbarWidth, float *scale, float *duration, std::string &filePath,
                      bool *isFilePathAdded, float *numberOfPoints, std::vector<Vector2> &fileDataPoints,
                      std::vector<Vector2> &dataPoints);
    bool checkPointValidity(Vector2 p, bool *showSettings);

  private:
    Vector2 window_position = {0, 0};
    Vector2 window_size = {0, 0};
    Vector2 max_window_size = {0, 0};
    Vector2 content_size = {0, 0};
    bool minimized = false;
    bool moving = false;
    bool resizing = false;
    Vector2 scroll;
    const char *title = "Title";

    void drawRandomPointGenerationComponent(Vector2 padding, Vector2 component_position, Vector2 component_size,
                                            Vector2 *scroll, float toolbarWidth, float *numberOfPoints,
                                            std::vector<Vector2> &dataPoints);
    void drawScaleComponent(Vector2 padding, Vector2 component_position, Vector2 component_size, Vector2 *scroll,
                            float *scale);
    void drawFileInputComponent(Vector2 padding, Vector2 component_position, Vector2 component_size, Vector2 *scroll,
                                bool *isFilePathAdded, std::string &filePath, std::vector<Vector2> &fileDataPoints,
                                std::vector<Vector2> &dataPoints, float toolbarHeight, float *scale, Rectangle scissor);
    void drawTimestepComponent(Vector2 padding, Vector2 component_position, Vector2 component_size, Vector2 *scroll,
                               float *duration);
    void computeScale(std::vector<Vector2> &fileDataPoints, float *scale, float toolbarHeight);
};

#endif