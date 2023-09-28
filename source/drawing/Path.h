#pragma once

#include "element.h"

#include<vector>


// Wrapper for a Gdiplus::GraphicsPath. Represents a sequence of Bezier curves, arcs, and lines.
class Path :
    public Element
{
public:
    Path(Gdiplus::Pen* pen) : pen(pen) { }
    Path(json path_json);
    ~Path();

    void get_bounding_box(Gdiplus::RectF** bounding_box);

    json to_json() override;

    void add_beziers(std::vector<Gdiplus::PointF> control_points);

    void set_pen(Gdiplus::Pen* new_pen) { pen = new_pen; }

private:
    void draw_one(Gdiplus::Graphics* graphics) override;

    Gdiplus::GraphicsPath* path = new Gdiplus::GraphicsPath;
    Gdiplus::Pen* pen;
};
