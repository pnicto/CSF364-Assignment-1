/**
 * @file jarvis_march.h
 * @brief Contains the declaration of the JarvisMarch class.
 *
 * This class implements the Jarvis March algorithm to compute the convex hull of a set of points.
 *
 */
#ifndef JARVIS_MARCH_H
#define JARVIS_MARCH_H

#include "convex_hull.h"

/**
 * @brief Represents the JarvisMarch class, implementing the Jarvis March algorithm for computing the convex hull of a
 * set of points.
 *
 * Usage:
 * 1. Create an instance of the JarvisMarch class with the points to be used to compute the convex hull. The convex hull
 * is computed in the constructor.
 * 2. Call the next() or previous() method to get the next or previous step in the convex hull computation process.
 * 3. Call the draw() method to draw the current step of the convex hull computation process.
 *
 */
class JarvisMarch : public ConvexHullAlgorithm
{
  private:
    /**
     * @brief Represents the orientation of any three points.
     *
     */
    enum Orientation
    {
        COLLINEAR,
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };
    /**
     * @brief Draws the convex hull using the points in JarvisMarch::convexHull.
     *
     */
    void drawConvexHull();
    /**
     * @brief Represents the convex hull.
     *
     */
    std::vector<Vector2> convexHull;
    /**
     * @brief Represents information about a step in the Jarvis March algorithm.
     *
     *
     */
    struct StepInfo
    {
        /**
         * @brief The index of the current point.
         *
         * This member variable stores the index of the current point being processed in the Jarvis March algorithm.
         */
        int currentPointIndex;

        /**
         * @brief The index of the next point.
         *
         * This member variable stores the index of the next point that forms the convex hull in the Jarvis March
         * algorithm.
         */
        int nextPointIndex;

        /**
         * @brief The index of the candidate point.
         *
         * This member variable stores the index of the candidate point being considered for inclusion in the convex
         * hull during the Jarvis March algorithm.
         */
        int candidatePointIndex;

        /**
         * @brief The current state of the convex hull.
         *
         * This member variable stores the current state of the convex hull being constructed during the Jarvis March
         * algorithm.
         */
        std::vector<Vector2> convexHull;
    };
    /**
     * @brief The collection of points to be used to compute the convex hull.
     *
     */
    std::vector<Vector2> points;
    /**
     * @brief Gets the index of the left most point in JarvisMarch::points.
     *
     * @return int The index of the left most point.
     */
    int getLeftMostPointIndex();
    /**
     * @brief Represents the size of the JarvisMarch::points.
     *
     */
    int n;
    /**
     * @brief Computes the orientation of three points.
     *
     * @param p The first point (Vector2) in consideration.
     * @param q The second point (Vector2) in consideration.
     * @param r The third point (Vector2) in consideration.
     * @return Orientation The orientation of the three points.
     */
    Orientation orientation(Vector2 p, Vector2 q, Vector2 r);
    /**
     * @brief Represents the steps in the convex hull computation process of the JarvisMarch object computed using
     * JarvisMarch::computeNextStep.
     *
     */
    std::vector<StepInfo> steps;
    /**
     * @brief Represents the current step in the JarvisMarch object.
     *
     */
    int currentStep;
    /**
     * @brief Computes all the steps in the convex hull computation process.
     *
     */
    void computeConvexHull();

  public:
    /**
     * @brief Construct a new Jarvis March object
     *
     * @param points The set of points to be used to compute the convex hull.
     */
    JarvisMarch(std::vector<Vector2> points);
    /**
     * @brief Destroy the Jarvis March object
     *
     */
    ~JarvisMarch();
    /**
     * @brief Checks if the JarvisMarch object has reached the last step in JarvisMarch::steps.
     *
     * @return true If the JarvisMarch object has reached the last step.
     * @return false If the JarvisMarch object has not reached the last step.
     */
    bool isFinished();
    /**
     * @brief Draws the JarvisMarch::currentStep of the JarvisMarch object.
     *
     */
    void draw();
    /**
     * @brief Gets the next step of the JarvisMarch object.
     *
     */
    void next();
    /**
     * @brief Gets the previous step of the JarvisMarch object.
     *
     */
    void previous();
    /**
     * @brief Gets the number of steps in the convex hull computation process.
     *
     * @return int The number of steps.
     */
    int getNumberOfSteps();
    /**
     * @brief Gets the current step that is being drawn.
     *
     * @return int The current step.
     */
    int getCurrentStep();
    /**
     * @brief Sets the current step to the specified value.
     *
     * @param step The step to set.
     */
    void setCurrentStep(int step);
    /**
     * @brief Show the legend for the visualization of the JarvisMarch computation.
     *
     */
    void showLegend(bool *showLegend, Vector2 *windowPosition, Vector2 *windowSize, Vector2 *maxWindowSize,
                    Vector2 *contentSize, Vector2 *scroll, bool *moving, bool *resizing, bool *minimized,
                    float toolbarHeight, float bottomBarHeight, const char *title);
};

#endif // JARVIS_MARCH_H