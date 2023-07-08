#pragma once
#include<vector>

#include "framework.h"
#include "element.h"
#include "Canvas.h"

namespace Constellation {

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
        ~Path();
        void draw(Canvas& canvas) override;
        void add_point(int xPos, int yPos);
        void add_points(Path& other);
        void pop_point();
        Gdiplus::Pen* pen;
    };

}
