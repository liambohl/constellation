#pragma once

#include "json.hpp"
#include "core/framework.h"

using json = nlohmann::json;


class Element
{
public:
    // Draw this element and transformed copies of it in accordance with the symmetry group.
    virtual void draw(Gdiplus::Graphics* graphics, std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms);
    // Draw only the original of this element
    virtual void draw_original(Gdiplus::Graphics* graphics);

    // Get a rectangle that neatly encloses this element. If the element is empty, return {}.
    virtual std::optional<Gdiplus::RectF> get_bounding_box(bool include_stroke=true) = 0;
    // Get a rectangle that neatly encloses these elements. If no elements, or all elements are empty, return {}.
    static std::optional<Gdiplus::RectF> get_bounding_box(std::vector<std::shared_ptr<Element>> elements, bool include_stroke = true);
    
    // Does this element have any overlap with the given rectangle?
    virtual bool intersects_rectangle(Gdiplus::RectF& rectangle) = 0;

    // Return true if the distance from the cursor to this element, in page space, is less than margin.
    virtual bool try_select(const Gdiplus::PointF& cursor_pos, float margin, float scale) = 0;

    // Transform (move, rotate, or scale) this element permanently
    virtual void transform(const Gdiplus::Matrix* transform) = 0;
    // Transform this element temporarily. The transformed copy will be overwritten later.
    // This is only for providing visual feedback while the user is in the middle of a transformation.
    virtual void transform_temp(const Gdiplus::Matrix* transform) = 0;

    virtual std::shared_ptr<Element> clone() = 0;

    virtual json to_json() = 0;

protected:
    Element() {}

    // Draw a single copy of thie element, unaffected by the symmetry group.
    virtual void draw_one(Gdiplus::Graphics* graphics) {};
};
