/**
 * @file kirk_patrick_seidel.cpp
 * @brief Contains the implementation of the KirkpatrickSeidel class.
 *
 */
#include "kirk_patrick_seidel.h"
#include "raygui.h"

bool KirkpatrickSeidel::compareVector2(Vector2 a, Vector2 b)
{
    if (a.x == b.x)
        return a.y < b.y;
    return a.x < b.x;
}

float KirkpatrickSeidel::calculateLim(float a, float b)
{
    float m = std::max(std::abs(a), std::abs(b));
    if (m <= 500)
        return 0.0001;
    else if (m <= 5000)
        return 0.001;
    return 0.01;
}

float KirkpatrickSeidel::medianOfMedians(std::vector<float> arr)
{
    int n = arr.size();
    std::vector<std::vector<float>> matrix(n / 5 + (n % 5 != 0));
    int i = 0, s = 0;

    while (i < n)
    {
        int count = 0;
        while (count < 5)
        {
            if (i < n)
            {
                matrix[s].push_back(arr[i]);
                i++;
            }
            else
                matrix[s].push_back(arr[n - 1]);
            count++;
        }
        s++;
    }

    for (std::vector<float> &V : matrix)
        sort(V.begin(), V.end());

    return matrix[matrix.size() / 2][2];
}

float KirkpatrickSeidel::quickSelect(std::vector<float> S, int rank)
{
    if (S.size() < rank)
        return 0;
    float x = medianOfMedians(S);
    int r = 0, dup = 0;
    std::vector<float> L, R;

    for (float e : S)
    {
        if (e < x)
        {
            L.push_back(e);
        }
        else if (e > x)
            R.push_back(e);
        else
            dup++;
    }
    dup--;
    r = L.size() + 1;

    if (r == rank || (r < rank && (r + dup) >= rank))
        return x;
    else if (r > rank)
        return quickSelect(L, rank);
    else
        return quickSelect(R, rank - (r + dup));
}

std::vector<Vector2> KirkpatrickSeidel::upperBridge(std::vector<Vector2> S, float L)
{
    int n = S.size();
    if (n <= 2)
        return S;

    std::vector<Vector2> candidates;

    // make pairs
    int pos = 0;
    std::vector<std::pair<int, int>> pairs;
    for (int i = 0; i < n / 2; i++)
    {
        int p_i = pos, p_j = n - 1 - pos;
        if (S[p_i].x > S[p_j].x)
        {
            p_i = p_j;
            p_j = pos;
        }

        pairs.push_back({p_i, p_j});
        pos++;
    }

    KirkpatrickSeidelStep step;
    step.state = currentState;
    step.type = PAIRS;
    step.arr = S;
    step.hullLineIndex = hullLineIndexHelper;
    step.upperBridgeLineIndex = upperBridges.size() - 1;
    step.lowerBridgeLineIndex = lowerBridges.size() - 1;

    for (auto pair : pairs)
    {
        step.pairs.push_back(S[pair.first]);
        step.pairs.push_back(S[pair.second]);
    }

    // point that could not be paired is added to candidates
    if (n % 2)
    {
        candidates.push_back(S[pos]);
        step.x_m = 1;
        step.p_k = S[pos];
    }

    steps.push_back(step);

    // calculate slopes
    std::vector<std::pair<std::pair<int, int>, float>> slopes;
    std::vector<float> only_slopes;
    for (auto p : pairs)
    {
        if (S[p.first].x == S[p.second].x)
            candidates.push_back(std::max(S[p.first], S[p.second], &KirkpatrickSeidel::compareVector2));
        else
        {
            slopes.push_back({p, (S[p.first].y - S[p.second].y) / (S[p.first].x - S[p.second].x)});
            only_slopes.push_back((S[p.first].y - S[p.second].y) / (S[p.first].x - S[p.second].x));
        }
    }

    // get median slope
    int k = slopes.size();

    float K = quickSelect(only_slopes, k / 2 + 1);

    KirkpatrickSeidelStep step4;
    step4.state = currentState;
    step4.type = MEDIAN_SLOPE;
    step4.arr = S;
    step4.hullLineIndex = hullLineIndexHelper;
    step4.upperBridgeLineIndex = upperBridges.size() - 1;
    step4.lowerBridgeLineIndex = lowerBridges.size() - 1;
    for (auto pair : pairs)
    {
        step4.pairs.push_back(S[pair.first]);
        step4.pairs.push_back(S[pair.second]);
    }
    step4.k = K;
    for (int i = 0; i < only_slopes.size(); i++)
    {
        if (FloatEquals(only_slopes[i], K))
        {
            step4.p_k = S[pairs[i].first];
            step4.p_m = S[pairs[i].second];
            break;
        }
    }
    steps.push_back(step4);

    // calculate intersections
    std::vector<float> intersections;
    for (auto point : S)
        intersections.push_back(point.y - (K * point.x));

    KirkpatrickSeidelStep step1;
    step1.state = currentState;
    step1.type = INTERCEPTS;
    step1.x_m = L;
    step1.arr = S;
    step1.k = K;
    step1.upperBridgeLineIndex = upperBridges.size() - 1;
    step1.lowerBridgeLineIndex = lowerBridges.size() - 1;
    step1.hullLineIndex = hullLineIndexHelper;
    steps.push_back(step1);

    Vector2 p_k, p_m;
    float m_int = -1 * std::numeric_limits<float>::infinity();

    for (int i = 0; i < n; i++)
    {
        if (intersections[i] - m_int > calculateLim(intersections[i], m_int))
        {
            m_int = intersections[i];
            p_k.x = S[i].x;
            p_k.y = S[i].y;
            p_m = p_k;
        }
        else if (abs(intersections[i] - m_int) <= calculateLim(intersections[i], m_int))
        {
            if (S[i].x > p_m.x)
            {
                p_m.x = S[i].x;
                p_m.y = S[i].y;
            }
            else if (S[i].x < p_k.x) // remove the else ? lite...
            {
                p_k.x = S[i].x;
                p_k.y = S[i].y;
            }
        }
    }

    // found the winning point
    KirkpatrickSeidelStep step2;
    step2.state = currentState;
    step2.type = INTERCEPTS_FINAL;
    step2.x_m = L;
    step2.p_k = p_k;
    step2.p_m = p_m;
    step2.k = K;
    step2.arr = S;
    step2.upperBridgeLineIndex = upperBridges.size() - 1;
    step2.lowerBridgeLineIndex = lowerBridges.size() - 1;
    step2.hullLineIndex = hullLineIndexHelper;
    steps.push_back(step2);

    // step 8 in slides
    if (p_k.x <= L && p_m.x > L)
        return {{p_k.x, p_k.y}, {p_m.x, p_m.y}};

    if (p_m.x <= L)
    {
        for (auto sl : slopes)
        {
            if (sl.second < K)
            {
                candidates.push_back(S[sl.first.first]);
                candidates.push_back(S[sl.first.second]);
            }
            else
                candidates.push_back(S[sl.first.second]);
        }
    }
    else if (p_k.x > L)
    {
        for (auto sl : slopes)
        {
            if (sl.second > K)
            {
                candidates.push_back(S[sl.first.first]);
                candidates.push_back(S[sl.first.second]);
            }
            else
                candidates.push_back(S[sl.first.first]);
        }
    }

    // final candidates step
    KirkpatrickSeidelStep step3;
    step3.state = currentState;
    step3.type = ADD_TO_CANDIDATES;
    step3.arr = candidates;
    step3.upperBridgeLineIndex = upperBridges.size() - 1;
    step3.lowerBridgeLineIndex = lowerBridges.size() - 1;
    step3.hullLineIndex = hullLineIndexHelper;
    steps.push_back(step3);

    return upperBridge(candidates, L);
}

std::vector<Vector2> KirkpatrickSeidel::upperHull(std::vector<Vector2> S)
{
    int n = S.size();
    if (n <= 2)
    {
        sort(S.begin(), S.end(), &KirkpatrickSeidel::compareVector2);
        if (n == 2)
        {
            KirkpatrickSeidelStep step2;
            step2.state = currentState;
            step2.type = (currentState == UPPER_HULL) ? UP_BRIDGE : LOW_BRIDGE;
            step2.p_k = S[0];
            step2.p_m = S[1];
            step2.upperBridgeLineIndex = upperBridges.size() - 1;
            step2.lowerBridgeLineIndex = lowerBridges.size() - 1;
            step2.hullLineIndex = hullLineIndexHelper;
            steps.push_back(step2);
        }
        return S;
    }

    // Get x_mid
    std::vector<float> x_s;
    for (auto v : S)
        x_s.push_back(v.x);
    float x_mid = quickSelect(x_s, n / 2 + 1);
    float x__mid = quickSelect(x_s, std::max(1, n / 2));
    x_mid = (x_mid + x__mid) / 2;
    x_mid = x_mid - 0.001; // IMPORTANT

    // drawing the median line is a step
    KirkpatrickSeidelStep step;
    step.state = currentState;
    step.type = LINE;
    step.x_m = x_mid;
    step.arr = S;
    // step.hullLineIndex = hullLineIndexHelper;
    step.upperBridgeLineIndex = upperBridges.size() - 1;
    step.lowerBridgeLineIndex = lowerBridges.size() - 1;
    steps.push_back(step);
    hullLineIndexHelper = steps.size() - 1;

    std::vector<Vector2> pq = upperBridge(S, x_mid);
    sort(pq.begin(), pq.end(), &KirkpatrickSeidel::compareVector2); // O(1) cause constant size

    if (currentState == UPPER_HULL && pq.size() > 1)
    {
        upperBridges.push_back({pq[0], pq[1]});
    }
    else if (currentState == LOWER_HULL && pq.size() > 1)
    {
        lowerBridges.push_back({pq[0], pq[1]});
    }

    // drawing the bridge is a step
    KirkpatrickSeidelStep step1;
    step1.state = currentState;
    step1.type = (currentState == UPPER_HULL) ? UP_BRIDGE : LOW_BRIDGE;
    step1.p_k = pq[0];
    step1.p_m = pq[1];
    step1.upperBridgeLineIndex = upperBridges.size() - 1;
    step1.lowerBridgeLineIndex = lowerBridges.size() - 1;
    step1.hullLineIndex = hullLineIndexHelper;
    steps.push_back(step1);

    std::vector<Vector2> L, R, res, temp_res;
    for (auto v : S)
    {
        if (v.x < pq[0].x)
            L.push_back(v);
        else if (v.x > pq[1].x)
            R.push_back(v);
        else if (pq[0].x != pq[1].x && v.x != pq[0].x &&
                 v.x != pq[1].x) // co-linear (lies btw p and q) but not vertical
        {
            float s1 = (pq[0].y - pq[1].y) / (pq[0].x - pq[1].x);
            float s2 = (pq[0].y - v.y) / (pq[0].x - v.x);
            if (abs(s1 - s2) < 0.0001) // s1 == s2
                temp_res.push_back(v);
        }
        else if (pq[0].x == pq[1].x && v.x == pq[0].x && v.y > pq[0].y &&
                 v.y < pq[1].y) // co-linear (lies btw p and q) and vertical
        {
            temp_res.push_back(v);
        }
    }
    L.push_back(pq[0]);
    R.push_back(pq[1]);

    sort(temp_res.begin(), temp_res.end(), &KirkpatrickSeidel::compareVector2); // O(hlogh)

    L = upperHull(L);
    R = upperHull(R);

    for (auto v : L)
        res.push_back(v);
    if (!Vector2Equals(res.back(), pq[0]))
        res.push_back(pq[0]);
    for (auto v : temp_res) // contains all co-linear points on the bridge
        res.push_back(v);
    if (!Vector2Equals(R[0], pq[1]))
        res.push_back(pq[1]);
    for (auto v : R)
        res.push_back(v);

    return res;
}

std::vector<Vector2> KirkpatrickSeidel::lowerHull(
    std::vector<Vector2> &S) // same as upper hull with negative y co-ordinates
{
    std::vector<Vector2> S_new, res;
    for (auto v : S)
        S_new.push_back({v.x, -v.y});

    res = upperHull(S_new);
    for (auto &v : res)
        v.y = -v.y;
    return res;
}

std::vector<Vector2> KirkpatrickSeidel::convexHull(std::vector<Vector2> &S)
{
    if (S.size() <= 2)
    {
        currentState = FINISH;
        KirkpatrickSeidelStep finalStep;
        finalStep.state = currentState;
        finalStep.type = OVER;
        steps.push_back(finalStep);
        return S;
    }
    std::vector<Vector2> uh, lh, res;

    currentState = UPPER_HULL;
    hullLineIndexHelper = 0;

    uh = upperHull(S);
    upperHullV = uh;

    currentState = LOWER_HULL;
    hullLineIndexHelper = 0;

    lh = lowerHull(S);

    currentState = MERGE;

    // remove common points (with upper hull) from lower hull where x = x_max
    int r = uh.size() - 1;
    while (r >= 0 && Vector2Equals(lh.back(), uh[r]))
    {
        lh.pop_back();
        r--;
    }

    // at x = x_min, upper hull might have a point (x_min,y1) and lower hull have (x_min,y2)
    // however, if y1 < y2, we swap the points
    int s = 0;
    while (s < lh.size() && s < uh.size() && lh[s].y > uh[s].y)
    {
        std::swap(uh[s], lh[s]);
        s++;
    }

    // does similar things as the upper block at x = x_max
    s = 1;
    while (s <= lh.size() && s <= uh.size() && lh[lh.size() - s].x == uh[uh.size() - s].x &&
           lh[lh.size() - s].y > uh[uh.size() - s].y)
    {
        std::swap(uh[uh.size() - s], lh[lh.size() - s]);
        s++;
    }

    reverse(lh.begin(), lh.end()); // to make a chain in final result

    float x_min = std::numeric_limits<float>::infinity();
    float x_max = -1 * std::numeric_limits<float>::infinity();
    for (auto v : S)
    {
        if (v.x > x_max)
            x_max = v.x;
        if (v.x < x_min)
            x_min = v.x;
    }

    std::vector<Vector2> temp_res; // stores points vertically co-linear at either ends

    for (auto v : S)
    {
        if (v.x == x_max || v.x == x_min)
            temp_res.push_back(v);
    }
    sort(temp_res.begin(), temp_res.end(), &KirkpatrickSeidel::compareVector2);

    // follwing block of code inserts into the result the points which are
    // vertically co-linear and between the first point in the upper hull and
    // last point in the (now reversed) lower hull
    // picture: these three form a vertical line at the left
    int i = 0;
    if (temp_res.size() > 2)
    {
        while (FloatEquals(temp_res[i].x, uh[0].x))
        {
            while (i < temp_res.size() && temp_res[i].y < uh[0].y && temp_res[i].y > lh.back().y &&
                   FloatEquals(temp_res[i].x, uh[0].x))
            {
                res.push_back(temp_res[i]);
                i++;
            }
            i++;
        }
    }

    // add upper hull to result
    for (auto v : uh)
    {
        if (res.size() == 0 || (res.size() > 0 && !Vector2Equals(res.back(), v)))
            res.push_back(v);
    }

    // the following block of code adds to the result, points vertically co-linear and between
    // the first point in (now reversed) lower hull and last point in upper hull
    // picture: these three form a vertical line at the right
    int p = 0;
    while (i < temp_res.size() && p < lh.size() && !Vector2Equals(temp_res[i], lh[p]))
    {
        i++;
        p++;
    }
    if (temp_res.size() > 2)
    {
        while (i < temp_res.size() && temp_res[i].y < uh.back().y && temp_res[i].y > lh[0].y &&
               temp_res[i].x == lh[0].x)
        {
            res.push_back(temp_res[i]);
            i++;
        }
    }

    // add lower hull to the result
    for (auto v : lh)
    {
        if (res.size() == 0 || (res.size() > 0 && !Vector2Equals(res.back(), v)))
            res.push_back(v);
    }

    // remove duplicates
    // example: we remove last 'a' from a->b->c->d->a
    int rr = 0;
    while (rr < res.size() && Vector2Equals(res[rr], res.back()))
    {
        res.pop_back();
        rr++;
    }

    currentState = FINISH;
    KirkpatrickSeidelStep finalStep;
    finalStep.state = currentState;
    finalStep.type = OVER;
    steps.push_back(finalStep);
    return res;
}

KirkpatrickSeidel::KirkpatrickSeidel(std::vector<Vector2> p)
{
    points = p;

    if (points.size() != 0)
        computeConvexHull();
}

void KirkpatrickSeidel::computeConvexHull()
{
    hull = convexHull(points);
    for (auto &p : lowerBridges)
    {
        p.first.y *= -1;
        p.second.y *= -1;
    }

    for (auto &s : steps)
    {
        if (s.state == LOWER_HULL)
        {
            for (auto &p : s.pairs)
            {
                p.y = -1 * p.y;
            }

            for (auto &p : s.arr)
            {
                p.y = -1 * p.y;
            }

            s.p_k.y *= -1;
            s.p_m.y *= -1;
            s.k *= -1;
        }
    }
}

KirkpatrickSeidel::~KirkpatrickSeidel()
{
    points.clear();
    hull.clear();
    steps.clear();
}

void KirkpatrickSeidel::draw()
{
    // BeginDrawing();
    if (steps[currentStep].state == LOWER_HULL)
        for (auto p : upperHullV)
            DrawCircleV(p, 5, BLUE);
    switch (steps[currentStep].type)
    {
    case LINE:
        // only considering points in arr, color them red
        for (Vector2 p : steps[currentStep].arr)
            DrawCircleV(p, 5, RED);

        // draw the median line
        DrawLineEx({steps[currentStep].x_m, 70}, {steps[currentStep].x_m, static_cast<float>(GetScreenHeight()) - 70},
                   2, RED);
        break;

    case PAIRS:

        for (Vector2 &p : steps[currentStep].arr)
        {
            DrawCircleV(p, 5, RED);
        }

        // draw all pair lines with orange
        for (int i = 1; i < steps[currentStep].pairs.size(); i += 2)
        {
            DrawLineV(steps[currentStep].pairs[i], steps[currentStep].pairs[i - 1], ORANGE);
        }

        // color the left out point purple
        if (steps[currentStep].x_m > 0)
            DrawCircleV(steps[currentStep].p_k, 5, PURPLE);
        break;

    case MEDIAN_SLOPE:
        // color relevant points red
        for (Vector2 &p : steps[currentStep].arr)
        {
            DrawCircleV(p, 5, RED);
        }

        // draw all pair lines with orange
        for (int i = 1; i < steps[currentStep].pairs.size(); i += 2)
        {
            DrawLineV(steps[currentStep].pairs[i], steps[currentStep].pairs[i - 1], ORANGE);
        }

        // color the left out point purple
        // if (steps[currentStep].x_m > 0)
        //     DrawCircleV(steps[currentStep].p_k, 5, PURPLE);

        // now draw the median slope line with pink

        if (!Vector2Equals(steps[currentStep].p_k, steps[currentStep].p_m))
        {
            DrawLineEx(steps[currentStep].p_k, steps[currentStep].p_m, 3, PINK);
        }
        else
            drawLine(steps[currentStep].p_k, steps[currentStep].k, steps[currentStep].x_m, PINK);

        break;

    case INTERCEPTS:
        // draw intercept lines with VIOLET
        for (Vector2 &p : steps[currentStep].arr)
        {
            DrawCircleV(p, 5, RED);
            drawLine(p, steps[currentStep].k, steps[currentStep].x_m, VIOLET);
        }
        break;

    case INTERCEPTS_FINAL:
        // draw the winning intercept with VIOLET
        for (Vector2 &p : steps[currentStep].arr)
        {
            DrawCircleV(p, 5, RED);
        }
        if (!Vector2Equals(steps[currentStep].p_k, steps[currentStep].p_m))
        {
            DrawLineV(steps[currentStep].p_k, steps[currentStep].p_m, VIOLET);
        }
        else
            drawLine(steps[currentStep].p_k, steps[currentStep].k, steps[currentStep].x_m, VIOLET);
        break;

    case ADD_TO_CANDIDATES:
        // show the candidates for the next recursive call
        for (Vector2 v : steps[currentStep].arr)
        {
            DrawCircleV(v, 5, PURPLE);
        }
        break;

    case UP_BRIDGE:
        // draw the upper_bridge
        DrawLineV(steps[currentStep].p_k, steps[currentStep].p_m, RED);
        break;

    case LOW_BRIDGE:
        // draw the lower bridge
        DrawLine(steps[currentStep].p_k.x, -1 * steps[currentStep].p_k.y, steps[currentStep].p_m.x,
                 -1 * steps[currentStep].p_m.y, RED);
        break;

    case OVER:
        break;
    }
    drawPrevSteps();
    // EndDrawing();
}

void KirkpatrickSeidel::next()
{

    if (currentStep < steps.size() - 1)
        currentStep++;
}

void KirkpatrickSeidel::previous()
{
    if (currentStep > 0)
        currentStep--;
}

bool KirkpatrickSeidel::isFinished()
{
    return currentStep >= steps.size() - 1;
}

void KirkpatrickSeidel::drawPrevSteps()
{
    KirkpatrickSeidelStep temp;
    KirkpatrickSeidelStep curr = steps[currentStep];
    switch (curr.state)
    {
    case INIT:
        break;

    case UPPER_HULL:
        // need to re-draw horizontal line and previous bridges
        if (curr.type != LINE)
        {
            temp = steps[curr.hullLineIndex];
            if (temp.type == LINE)
            {
                DrawLineEx({temp.x_m, 70}, {temp.x_m, static_cast<float>(GetScreenHeight()) - 70}, 2, RED);
            }
        }
        for (int i = 0; i <= curr.upperBridgeLineIndex; i++)
        {
            DrawLineEx(upperBridges[i].first, upperBridges[i].second, 2, GREEN);
            DrawCircleV(upperBridges[i].first, 5, BLUE);
            DrawCircleV(upperBridges[i].second, 5, BLUE);
        }

        break;

    case LOWER_HULL:
        for (int i = 1; i < upperHullV.size(); i++)
            DrawLineEx(upperHullV[i], upperHullV[i - 1], 2, GREEN);

        if (curr.type != LINE)
        {
            temp = steps[curr.hullLineIndex];
            if (temp.type == LINE)
            {
                DrawLineEx({temp.x_m, 70}, {temp.x_m, static_cast<float>(GetScreenHeight()) - 70}, 2, RED);
            }
        }
        for (int i = 0; i <= curr.lowerBridgeLineIndex; i++)
        {
            DrawLineEx(lowerBridges[i].first, lowerBridges[i].second, 2, GREEN);
            DrawCircleV(lowerBridges[i].first, 5, BLUE);
            DrawCircleV(lowerBridges[i].second, 5, BLUE);
        }

        break;

    case MERGE:
        break;

    case FINISH:

        // draw the entire hull
        DrawCircleV(hull[0], 5, BLUE);
        for (int i = 1; i < hull.size(); i++)
        {
            DrawLineEx(hull[i], hull[i - 1], 2, GREEN);
            DrawCircleV(hull[i], 5, BLUE);
        }
        if (hull.size() >= 2)
            DrawLineEx(hull[0], hull.back(), 2, GREEN);

        break;
    }
}

// make more responsive !!!
void KirkpatrickSeidel::drawLine(Vector2 p, float slope, float x_mid, Color c)
{
    // Calculate the y-coordinate of the intersection point
    float y_intersect = p.y + slope * (x_mid - p.x);

    if (y_intersect < 70)
    {
        y_intersect = 70;
        x_mid = p.x + (y_intersect - p.y) / slope;
    }
    else if (y_intersect > GetScreenHeight() - 70)
    {
        y_intersect = GetScreenHeight() - 70;
        x_mid = p.x + (y_intersect - p.y) / slope;
    }

    // Draw the line
    DrawLineV(p, {x_mid, y_intersect}, c);
}

int KirkpatrickSeidel::getNumberOfSteps()
{
    return steps.size();
}

int KirkpatrickSeidel::getCurrentStep()
{
    return currentStep;
}

void KirkpatrickSeidel::setCurrentStep(int step)
{
    currentStep = step;
}

std::vector<Vector2> KirkpatrickSeidel::exportHull()
{
    return hull;
}

void KirkpatrickSeidel::showLegend(bool *showLegend, Vector2 *windowPosition, Vector2 *windowSize,
                                   Vector2 *maxWindowSize, Vector2 *contentSize, Vector2 *scroll, bool *moving,
                                   bool *resizing, bool *minimized, float toolbarHeight, float bottomBarHeight,
                                   const char *title)
{
    float statusBarHeight = 24.0f, closeButtonSize = 18.0f;
    if (*showLegend)
    {
        int closeTitleSizeDeltaHalf = (statusBarHeight - closeButtonSize) / 2;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !(*moving) && !(*resizing))
        {
            Vector2 mousePosition = GetMousePosition();

            Rectangle titleCollisionRect = {(*windowPosition).x, (*windowPosition).y,
                                            (*windowSize).x - (closeButtonSize + closeTitleSizeDeltaHalf),
                                            statusBarHeight};
            Rectangle resizeCollisionRect = {(*windowPosition).x + (*windowSize).x - 20.0f,
                                             (*windowPosition).y + (*windowSize).y - 20.0f, 20.0f, 20.0f};

            if (CheckCollisionPointRec(mousePosition, titleCollisionRect))
            {
                (*moving) = true;
            }
            else if (!(*minimized) && CheckCollisionPointRec(mousePosition, resizeCollisionRect))
            {
                (*resizing) = true;
            }
        }

        if ((*moving))
        {
            Vector2 mouseDelta = GetMouseDelta();
            (*windowPosition).x += mouseDelta.x;
            (*windowPosition).y += mouseDelta.y;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                (*moving) = false;

                if ((*windowPosition).x < 0.0f)
                    (*windowPosition).x = 10.0f;
                else if ((*windowPosition).x > GetScreenWidth() - (*windowSize).x)
                    (*windowPosition).x = GetScreenWidth() - (*windowSize).x - 10.0f;
                if ((*windowPosition).y < toolbarHeight)
                    (*windowPosition).y = toolbarHeight + 10;
                else if ((*windowPosition).y > GetScreenHeight() - toolbarHeight - bottomBarHeight)
                    (*windowPosition).y = GetScreenHeight() - bottomBarHeight - statusBarHeight - 10.0f;
            }
        }
        else if ((*resizing))
        {
            Vector2 mouseDelta = GetMouseDelta();
            (*windowSize).x += mouseDelta.x;
            (*windowSize).y += mouseDelta.y;

            if ((*windowSize).x < 100.0f)
                (*windowSize).x = 100.0f;
            else if ((*windowSize).x > GetScreenWidth() - 10.0f)
                (*windowSize).x = GetScreenWidth() - 10.0f;
            if ((*windowSize).y < 100.0f)
                (*windowSize).y = 100.0f;
            else if ((*windowSize).y > GetScreenHeight() - toolbarHeight - bottomBarHeight - 10.0f)
                (*windowSize).y = GetScreenHeight() - toolbarHeight - bottomBarHeight - 10.0f;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                (*resizing) = false;
            }
        }

        if ((*minimized))
        {
            GuiStatusBar((Rectangle){(*windowPosition).x, (*windowPosition).y, (*windowSize).x, statusBarHeight},
                         title);

            if (GuiButton((Rectangle){(*windowPosition).x + (*windowSize).x - closeButtonSize - closeTitleSizeDeltaHalf,
                                      (*windowPosition).y + closeTitleSizeDeltaHalf, closeButtonSize, closeButtonSize},
                          "#120#"))
            {
                (*minimized) = false;
                (*windowSize) = (*maxWindowSize);
            }
        }
        else
        {
            (*minimized) = GuiWindowBox(
                (Rectangle){(*windowPosition).x, (*windowPosition).y, (*windowSize).x, (*windowSize).y}, title);
            if ((*minimized))
            {
                (*windowSize) = {(*maxWindowSize).x, statusBarHeight};
            }

            Rectangle scissor = {0};
            GuiScrollPanel((Rectangle){(*windowPosition).x, (*windowPosition).y + statusBarHeight, (*windowSize).x,
                                       (*windowSize).y - statusBarHeight},
                           NULL, (Rectangle){(*windowPosition).x, (*windowSize).y, (*contentSize).x, (*contentSize).y},
                           scroll, &scissor);

            bool requireScissor = (*windowSize).x < (*contentSize).x || (*windowSize).y < (*contentSize).y;

            if (requireScissor)
            {
                BeginScissorMode(scissor.x, scissor.y, scissor.width, scissor.height);
            }

            DrawCircleV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 50.0f + (*scroll).y}, 5,
                        BLUE);
            GuiLabel(
                {(*windowPosition).x + 30.0f + (*scroll).x, (*windowPosition).y + 35.0f + (*scroll).y, 300.0f, 30.0f},
                " - Convex Hull Point");

            DrawCircleV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 75.0f + (*scroll).y}, 5,
                        PURPLE);
            GuiLabel(
                {(*windowPosition).x + 30.0f + (*scroll).x, (*windowPosition).y + 60.0f + (*scroll).y, 300.0f, 30.0f},
                " - Candidate Point");

            DrawCircleV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 100.0f + (*scroll).y}, 5,
                        RED);
            GuiLabel(
                {(*windowPosition).x + 30.0f + (*scroll).x, (*windowPosition).y + 85.0f + (*scroll).y, 300.0f, 30.0f},
                " - Current Check Points");

            DrawLineV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 125.0f + (*scroll).y},
                      {(*windowPosition).x + 80.0f + (*scroll).x, (*windowPosition).y + 125.0f + (*scroll).y}, GREEN);
            GuiLabel(
                {(*windowPosition).x + 90.0f + (*scroll).x, (*windowPosition).y + 110.0f + (*scroll).y, 300.0f, 30.0f},
                " - Convex Hull Line");

            DrawLineV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 150.0f + (*scroll).y},
                      {(*windowPosition).x + 80.0f + (*scroll).x, (*windowPosition).y + 150.0f + (*scroll).y}, RED);
            GuiLabel(
                {(*windowPosition).x + 90.0f + (*scroll).x, (*windowPosition).y + 135.0f + (*scroll).y, 300.0f, 30.0f},
                " - Median Line");

            DrawLineV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 175.0f + (*scroll).y},
                      {(*windowPosition).x + 80.0f + (*scroll).x, (*windowPosition).y + 175.0f + (*scroll).y}, PINK);
            GuiLabel(
                {(*windowPosition).x + 90.0f + (*scroll).x, (*windowPosition).y + 160.0f + (*scroll).y, 300.0f, 30.0f},
                " - Median Slope Line");

            DrawLineV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 200.0f + (*scroll).y},
                      {(*windowPosition).x + 80.0f + (*scroll).x, (*windowPosition).y + 200.0f + (*scroll).y}, ORANGE);
            GuiLabel(
                {(*windowPosition).x + 90.0f + (*scroll).x, (*windowPosition).y + 185.0f + (*scroll).y, 300.0f, 30.0f},
                " - Paired Point Line Segment");

            DrawLineV({(*windowPosition).x + 20.0f + (*scroll).x, (*windowPosition).y + 225.0f + (*scroll).y},
                      {(*windowPosition).x + 80.0f + (*scroll).x, (*windowPosition).y + 225.0f + (*scroll).y}, VIOLET);
            GuiLabel(
                {(*windowPosition).x + 90.0f + (*scroll).x, (*windowPosition).y + 210.0f + (*scroll).y, 300.0f, 30.0f},
                " - Intercept Line");

            if (requireScissor)
            {
                EndScissorMode();
            }

            GuiDrawIcon(71, (*windowPosition).x + (*windowSize).x - 20, (*windowPosition).y + (*windowSize).y - 20, 1,
                        WHITE);
        }
    }
}