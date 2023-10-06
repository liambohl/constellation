#pragma once

#include "element.h"

#include<vector>


// Wrapper for a Gdiplus::GraphicsPath. Represents a sequence of Bezier curves, arcs, and lines.
class Path :
    public Element
{
public:
    Path(Gdiplus::Pen* pen) : pen(pen) {}
    Path(json path_json);
    Path(const Path& other) : path(other.path->Clone()), pen(other.pen->Clone()) {}
    ~Path();

    std::optional<Gdiplus::RectF> get_bounding_box() override;

    bool try_select(const Gdiplus::PointF& cursor_pos, float margin, float scale) override;
    
    void translate(const Gdiplus::PointF& delta) override;

    std::shared_ptr<Element> clone() override   ;

    json to_json() override;

    void add_beziers(std::vector<Gdiplus::PointF> control_points);

    void set_pen(Gdiplus::Pen* new_pen) { pen = new_pen; }

private:
    void draw_one(Gdiplus::Graphics* graphics) override;

    Gdiplus::GraphicsPath* path = new Gdiplus::GraphicsPath;
    Gdiplus::Pen* pen;
};
