#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

struct Vector2
{
    float x, y;
};
bool operator==(const Vector2 &lhs, const Vector2 &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

int orientation(Vector2 p, Vector2 q, Vector2 r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
    {
        return 0;
    }
    return (val > 0) ? 1 : 2;
}

vector<Vector2> computeConvexHull(vector<Vector2> points)
{
    int n = points.size();
    vector<Vector2> convexHull;

    if (n < 3)
    {
        return points;
    }

    int left = 0;
    for (int i = 1; i < n; i++)
    {
        if (points[i].x < points[left].x)
        {
            left = i;
        }
        else if (points[i].x == points[left].x && points[i].y < points[left].y)
        {
            left = i;
        }
    }
    int current = left, next;

    do
    {
        convexHull.push_back(points[current]);
        next = (current + 1) % n;

        for (int i = 0; i < n; i++)
        {
            if (orientation(points[current], points[i], points[next]) == 2)
            {
                next = i;
            }
        }
        current = next;
    } while (current != left);
    return convexHull;
}

int main()
{
    vector<Vector2> points;
    const std::string inputFilePath = "./in.txt";
    std::ifstream istream(inputFilePath);
    float x, y;
    char openParenthesis = '(', closeParenthesis = ')', separator = ',';
    while (istream >> openParenthesis >> x >> separator >> y >> closeParenthesis)
    {
        points.push_back({x, y});
    }
    auto result = computeConvexHull(points);
    const std::string outputFilePath = "./jarvisout.txt";
    std::ofstream ostream(outputFilePath);
    for (auto &point : result)
    {
        ostream << fixed << std::setprecision(0) << openParenthesis << point.x << separator << point.y
                << closeParenthesis << endl;
    }
    return 0;
}
