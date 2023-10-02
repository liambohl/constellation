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
	void draw_original(Gdiplus::Graphics* graphics);

	void set_symmetry_group(std::shared_ptr<SymmetryGroup> group) { symmetry_group = group; }
	std::shared_ptr<SymmetryGroup> get_symmetry_group() { return symmetry_group; }

	std::optional<Gdiplus::RectF> get_bounding_box() { return elements.get_bounding_box(); }

	void add_element(std::shared_ptr<Element> element) { elements.add_element(element); }
	bool remove_element(std::shared_ptr<Element> element) { return elements.remove_element(element); }

	std::shared_ptr<Element> select_element(const Gdiplus::PointF& cursor_pos, float margin, float scale) { return elements.select_in_group(cursor_pos, margin, scale); }
	std::vector<std::shared_ptr<Element>> select_all() { return elements.select_all(); }

	json to_json();

private:
	Group elements;

	Gdiplus::Color background;				// "transparent" color shown where there are no elements

	std::shared_ptr<SymmetryGroup> symmetry_group;
};
