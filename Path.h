#pragma once
#include<vector>

#include "Canvas.h"
#include "element.h"
#include "framework.h"


// Discribes a cubic Bezier curve or a sequence of cubic Bezier curves.
// A line segment is represented as a cubic Bezier curve with control points
// p0, p1, p2, and p3, where p0=p1 and p2=p3.
class Path :
    public Element
{
private:
    // Every third control point, starting with the first, is a point on the path.
    // The other points simply "pull" the curve toward them.
    // Thus, any path should have n=3k+1 points for some positive integer k.
    std::vector<Gdiplus::Point> control_points;
    INT n_points = 0;

public:
    Path(Gdiplus::Pen* pen) : pen(pen) {}
    Path(json path_json);
    //Path(const Path& other);
    ~Path();
    void draw(Canvas& canvas) override;

    json to_json() override;

    void add_point(int xPos, int yPos, int count=1);
    void add_points(Path& other);
    void pop_point(int count=1);
    Gdiplus::Point top();

    Gdiplus::Pen* pen;
};
