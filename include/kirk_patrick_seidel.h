#ifndef KIRK_PATRICK_SEIDEL_H
#define KIRK_PATRICK_SEIDEL_H

#include "convex_hull.h"

class Kirk : public ConvexHullAlgorithm
{
  public:
    enum miniState
    {
        LINE,
        PAIRS,
        INTERCEPTS,
        MEDIAN_SLOPE,
        INTERCEPTS_FINAL,
        ADD_TO_CANDIDATES,
        UP_BRIDGE,
        LOW_BRIDGE,
        OVER
    };

    enum State
    {
        INIT,
        UPPER_HULL,
        LOWER_HULL,
        MERGE,
        FINISH
    };

    Kirk(std::vector<Vector2> p);
    ~Kirk();

    void draw();
    void next();
    void previous();
    bool isFinished();
    void drawPrevSteps();
    void drawline(Vector2 p, float slope, float x_mid, Color c);
    static bool compareVector2(Vector2 a, Vector2 b);
    static inline float calculateLim(float a, float b);
    int getNumberOfSteps();
    int getCurrentStep();
    void setCurrentStep(int step);
    void showLegend(bool *showLegend, Vector2 *windowPosition, Vector2 *windowSize, Vector2 *maxWindowSize,
                    Vector2 *contentSize, Vector2 *scroll, bool *moving, bool *resizing, bool *minimized,
                    float toolbarHeight, float bottomBarHeight, const char *title);

  private:
    float median_of_medians(std::vector<float> arr);
    float quick_select(std::vector<float> S, int rank);
    std::vector<Vector2> upper_bridge(std::vector<Vector2> S, float L);
    std::vector<Vector2> upper_hull(std::vector<Vector2> S);
    std::vector<Vector2> lower_hull(std::vector<Vector2> &S);
    std::vector<Vector2> convex_hull(std::vector<Vector2> &S);
    void computeConvexHull();

    std::vector<Vector2> points;
    std::vector<Vector2> hull;
    std::vector<Vector2> upperHull;
    std::vector<std::pair<Vector2, Vector2>> upperBridges;
    std::vector<std::pair<Vector2, Vector2>> lowerBridges;

    State currentState = State::INIT;
    miniState currentMiniState = miniState::LINE;
    float x_mid;

    struct Step
    {
        State state;
        miniState type;
        std::vector<Vector2> arr;
        std::vector<Vector2> pairs;
        Vector2 p_k, p_m;
        float x_m = 0, k = 0;
        int hullLineIndex = 0;
        int upperBridgeLineIndex = 0;
        int lowerBridgeLineIndex = 0;
    };

    std::vector<Step> steps;
    int currentStep = 0;
    int hullLineIndexHelper = 0;
    // float lim = 0.0001;
    // int uhIndexHelper = 0;
    // int lhIndexHelper = 0;
    // int ubIndexHelper = 0;
    // int lbIndexHelper = 0;
};

#endif // KIRK_PATRICK_SEIDEL_H