#ifndef SETTINGS_H
#define SETTINGS_H

#include "raylib.h"
#include <iostream>
#include <vector>

class Settings
{
  public:
    Settings(Vector2 *position, Vector2 *size, bool *min, bool *mov, bool *resiz, Vector2 *content_size,
             const char *title);
    ~Settings();
    void showSettings(bool *showSettings, float toolbarWidth, float *numberOfPoints, std::vector<Vector2> &dataPoints);
    bool checkPointValidity(Vector2 p, bool *showSettings);

  private:
    Vector2 window_position = {0, 0};
    Vector2 window_size = {0, 0};
    Vector2 max_window_size = {0, 0};
    bool minimized = false;
    bool moving = false;
    bool resizing = false;
    Vector2 content_size = {0, 0};
    Vector2 scroll;
    const char *title = "Title";
};

#endif