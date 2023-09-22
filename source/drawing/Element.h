#pragma once

#include "json.hpp"
#include "core/framework.h"

using json = nlohmann::json;


class Element
{
public:
	// Draw this element and transformed copies of it in accordance with the symmetry group.
	virtual void draw(Gdiplus::Graphics* graphics, std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms);

	virtual void get_bounding_box(Gdiplus::RectF** bounding_box) = 0;

	virtual json to_json() = 0;

protected:
	Element() {}

	// Draw a single copy of thie element, unaffected by the symmetry group.
	virtual void draw_one(Gdiplus::Graphics* graphics) {};
};
