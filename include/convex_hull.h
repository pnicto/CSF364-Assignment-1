/**
 * @file convex_hull.h
 * @brief Contains the declaration of the abstract class ConvexHullAlgorithm.
 *
 */
#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stack>
#include <utility>
#include <vector>

/**
 * @brief Abstract class that represents a convex hull algorithm.
 *
 * This class is an abstract class that represents a convex hull algorithm. It contains the common methods and
 * attributes that all convex hull algorithms should have.
 */
class ConvexHullAlgorithm
{
  private:
    /**
     * @brief The collection of points to be used to compute the convex hull.
     *
     */
    std::vector<Vector2> points;
    /**
     * @brief Represents the convex hull.
     *
     */
    std::vector<Vector2> convexHull;
    /**
     * @brief Computes all the steps in the convex hull computation process.
     *
     */
    virtual void computeConvexHull() = 0;
    /**
     * @brief Represents the current step in the JarvisMarch object.
     *
     */
    int currentStep;

  public:
    /**
     * @brief Destroy the ConvexHullAlgorithm object
     *
     */
    virtual ~ConvexHullAlgorithm() = default;
    /**
     * @brief Checks if the ConvexHullAlgorithm object has reached the last step.
     *
     * @return true If the ConvexHullAlgorithm object has reached the last step.
     * @return false If the ConvexHullAlgorithm object has not reached the last step.
     */
    virtual bool isFinished() = 0;
    /**
     * @brief Draws the current step of the ConvexHullAlgorithm object.
     *
     */
    virtual void draw() = 0;
    /**
     * @brief Gets the next step of the ConvexHullAlgorithm object.
     *
     */
    virtual void next() = 0;
    /**
     * @brief Gets the previous step of the ConvexHullAlgorithm object.
     *
     */
    virtual void previous() = 0;
    /**
     * @brief Get the number of steps in the convex hull computation process.
     *
     * @return int The number of steps.
     */
    virtual int getNumberOfSteps() = 0;
    /**
     * @brief Gets the current step that is being drawn.
     *
     * @return int The current step.
     */
    virtual int getCurrentStep() = 0;
    /**
     * @brief Sets the current step to the specified value.
     *
     * @param stepIndex The step to set.
     */
    virtual void setCurrentStep(int stepIndex) = 0;
    /**
     * @brief Show the legend for the visualization of the ConvexHullAlgorithm computation.
     *
     * @param showLegend The variable used to indicate if the legend is to be displayed
     * @param windowPosition The position of the legend window on the screen
     * @param windowSize The size of the legend window
     * @param maxWindowSize The size of the legend window in the maximized state
     * @param contentSize The size of the content window inside the legend window
     * @param scroll The object used to keep track of current position after scrolling
     * @param moving The variable used to indicate if the floating window is moving
     * @param resizing The variable used to indicate if the floating window is resizing
     * @param minimized The variable used to indicate if the floating window is minimized
     * @param toolbarHeight The height of the toolbar
     * @param bottomBarHeight The height of the bottomBar
     * @param title The title for the floating window
     */
    virtual void showLegend(bool *showLegend, Vector2 *windowPosition, Vector2 *windowSize, Vector2 *maxWindowSize,
                            Vector2 *contentSize, Vector2 *scroll, bool *moving, bool *resizing, bool *minimized,
                            float toolbarHeight, float bottomBarHeight, const char *title) = 0;
    /**
     * @brief Get all the points on the final convex hull.
     *
     * @return std::vector<Vector2> All the points on the convex hull.
     */
    virtual std::vector<Vector2> exportHull() = 0;
};

#endif // CONVEX_HULL_H