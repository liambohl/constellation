#pragma once
#include <vector>

#include "Element.h"
#include "Group.h"
#include "json.hpp"
#include "core/framework.h"
#include "symmetry/SymmetryGroup.h"

using json = nlohmann::json;

// Represents an entire drawing with elements (shapes, curves, etc.) and a pattern
class Drawing
{
public:
	Drawing();
	Drawing(json drawing_json);

	void draw(Gdiplus::Graphics* graphics);

	void set_symmetry_group(std::shared_ptr<SymmetryGroup> group) { symmetry_group = group; }
	std::shared_ptr<SymmetryGroup> get_symmetry_group() { return symmetry_group; }

	Gdiplus::RectF* get_bounding_box();

	json to_json();

	// Pattern pattern;
	Group elements;

private:
	Gdiplus::Color background;				// "transparent" color shown where there are no elements

	std::shared_ptr<SymmetryGroup> symmetry_group;
};
