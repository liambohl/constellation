#pragma once
#include <vector>

#include "Element.h"
#include "framework.h"
#include "Group.h"
#include "json.hpp"

using json = nlohmann::json;

// Represents an entire drawing with elements (shapes, curves, etc.) and a pattern
class Drawing
{
public:
	Drawing();
	Drawing(json drawing_json);

	void draw(Gdiplus::Graphics* graphics);

	Gdiplus::RectF* get_bounding_box();

	json to_json();

	// Pattern pattern;
	Group elements;

private:
	Gdiplus::Color background;				// "transparent" color shown where there are no elements
};
