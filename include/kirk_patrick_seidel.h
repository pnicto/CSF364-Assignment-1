/**
 * @file kirk_patrick_seidel.h
 * @brief Contains the declaration of the KirkpatrickSeidel class.
 *
 * This class implements the Kirkpatrick-Seidel algorithm to compute the convex hull of a set of points.
 *
 */
#ifndef KIRK_PATRICK_SEIDEL_H
#define KIRK_PATRICK_SEIDEL_H

#include "convex_hull.h"

/**
 * @brief Represents the KirkpatrickSeidel class, implementing the Kirkpatrick-Seidel algorithm for computing the convex
 * hull of a set of points.
 *
 * Usage:
 * 1. Create an instance of the KirkpatrickSeidel class with the points to be used to compute the convex hull. The
 * convex hull is computed in the constructor.
 * 2. Call the next() or previous() method to get the next or previous step in the convex hull computation process.
 * 3. Call the draw() method to draw the current step of the convex hull computation process.
 *
 */
class KirkpatrickSeidel : public ConvexHullAlgorithm
{
  public:
    /**
     * @brief Represents the sub/mini state during a step in the algorithm
     *
     */
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

    /**
     * @brief Represents the state during a step in the algorithm
     *
     */
    enum State
    {
        INIT,
        UPPER_HULL,
        LOWER_HULL,
        MERGE,
        FINISH
    };

    /**
     * @brief Construct a new Kirkpatrick Seidel object
     *
     * @param p Set of input points
     */
    KirkpatrickSeidel(std::vector<Vector2> p);
    /**
     * @brief Destroy the Kirkpatrick Seidel object
     *
     */
    ~KirkpatrickSeidel();

    /**
     * @brief Draws the KirkpatrickSeidel::currentStep of the KirkpatrickSeidel object
     *
     */
    void draw();
    /**
     * @brief Gets the next step of the KirkpatrickSeidel object
     *
     */
    void next();
    /**
     * @brief Gets the previous step of the KirkpatrickSeidel object
     *
     */
    void previous();
    /**
     * @brief Checks if the KirkpatrickSeidel object has reached the last step in KirkpatrickSeidel::steps
     *
     * @return true If the KirkpatrickSeidel has reached the last step
     * @return false If the KirkpatrickSeidel object has not reached the last step
     */
    bool isFinished();
    /**
     * @brief Draws the previous relevant steps for currentStep
     *
     */
    void drawPrevSteps();
    /**
     * @brief Draws a line from a given point with a given slope till it intersects x = x_mid
     *
     * @param p The given point
     * @param slope The given slope
     * @param x_mid The middle vertical line (x=x_mid)
     * @param c The color
     */
    void drawline(Vector2 p, float slope, float x_mid, Color c);
    /**
     * @brief Comparator function for Vector2
     *
     * @param a Vector2 operand
     * @param b Vector2 operand
     * @return true if (a < b)
     * @return false otherwise
     */
    static bool compareVector2(Vector2 a, Vector2 b);
    /**
     * @brief Calculates an error threshold for float comparisons
     *
     * @param a The float operand
     * @param b The float operand
     * @return float threshold
     */
    static inline float calculateLim(float a, float b);
    /**
     * @brief Get the Number Of Steps in the convex hull computation
     *
     * @return int number of steps
     */
    int getNumberOfSteps();
    /**
     * @brief Get the Current Step that is being drawn
     *
     * @return int The current step
     */
    int getCurrentStep();
    /**
     * @brief Sets the current step to the specified value.
     *
     * @param step The step to set.
     */
    void setCurrentStep(int step);
    /**
     * @brief Show the legend for the visualization of the KirkpatrickSeidel computation.
     *
     */
    void showLegend(bool *showLegend, Vector2 *windowPosition, Vector2 *windowSize, Vector2 *maxWindowSize,
                    Vector2 *contentSize, Vector2 *scroll, bool *moving, bool *resizing, bool *minimized,
                    float toolbarHeight, float bottomBarHeight, const char *title);

  private:
    /**
     * @brief Helper for KirkpatrickSeidel::quick_select(), finds a pivot point
     *
     * @param arr The input array
     * @return float The pivot
     */
    float median_of_medians(std::vector<float> arr);
    /**
     * @brief Selects element of given rank from a given unsorted array in O(n) time
     *
     * @param S The given array
     * @param rank The given rank
     * @return float Selected element
     */
    float quick_select(std::vector<float> S, int rank);
    /**
     * @brief Calculates upper bridge for given set of points
     *
     * @param S Given set of points
     * @param L The middle vertical line
     * @return std::vector<Vector2> Points on the upper bridge
     */
    std::vector<Vector2> upper_bridge(std::vector<Vector2> S, float L);
    /**
     * @brief Calculates upper hull for a given set of points
     *
     * @param S Given set of points
     * @return std::vector<Vector2> Points on the upper hull
     */
    std::vector<Vector2> upper_hull(std::vector<Vector2> S);
    /**
     * @brief Calculates lower hull for a given set of points
     *
     * @param S Given set of points
     * @return std::vector<Vector2> Points on the lower hull
     */
    std::vector<Vector2> lower_hull(std::vector<Vector2> &S);
    /**
     * @brief Calculates the convex hull for a given set of points
     *
     * @param S Given set of points
     * @return std::vector<Vector2> Points on the convex hull
     */
    std::vector<Vector2> convex_hull(std::vector<Vector2> &S);
    /**
     * @brief Helper function used in the constructor to aid visulaization
     *
     */
    void computeConvexHull();

    /**
     * @brief The collection of points to be used to compute the convex hull.
     *
     */
    std::vector<Vector2> points;
    /**
     * @brief The calculated convex hull
     *
     */
    std::vector<Vector2> hull;
    /**
     * @brief The calculated upper hull
     *
     */
    std::vector<Vector2> upperHull;
    /**
     * @brief Array of all calculated upper bridges
     *
     */
    std::vector<std::pair<Vector2, Vector2>> upperBridges;
    /**
     * @brief Array of all calculated lower bridges
     *
     */
    std::vector<std::pair<Vector2, Vector2>> lowerBridges;

    /**
     * @brief Holds the current state during a step in the algorithm
     *
     */
    State currentState = State::INIT;

    /**
     * @brief Holds the informations about a step in KirkpatrickSeidel algorithm
     *
     */
    struct Step
    {
        /**
         * @brief State of the step
         *
         */
        State state;
        /**
         * @brief Mini/Sub State of the step
         *
         */
        miniState type;
        /**
         * @brief Holds various values depending upon the State and miniState
         *
         */
        std::vector<Vector2> arr;
        /**
         * @brief Holds pairs of points formed during pairing step
         *
         */
        std::vector<Vector2> pairs;
        /**
         * @brief Points that form the median slope pair
         *
         */
        Vector2 p_k, p_m;
        /**
         * @brief x co-ordinate of the middle vertical line
         *
         */
        float x_m = 0;
        /**
         * @brief Slope
         *
         */
        float k = 0;
        /**
         * @brief Index of the latest step with miniState LINE
         *
         */
        int hullLineIndex = 0;
        /**
         * @brief Index upto which upperBridgeLine needs to be drawn for the current step
         *
         */
        int upperBridgeLineIndex = 0;
        /**
         * @brief Index upto which lowerBridgeLine needs to be drawn for the current step
         *
         */
        int lowerBridgeLineIndex = 0;
    };

    /**
     * @brief Holds steps in the computation of convex hull
     *
     */
    std::vector<Step> steps;
    /**
     * @brief The current step in the visualization process
     *
     */
    int currentStep = 0;
    /**
     * @brief Helper variable to calculate upperBridgeLine for a Step object
     *
     */
    int hullLineIndexHelper = 0;
};

#endif // KIRK_PATRICK_SEIDEL_H