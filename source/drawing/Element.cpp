#include "Element.h"

#include <limits>


// For elements that are not groups, the draw method should draw
// a transformed copy of the element for each transform.
void Element::draw(Gdiplus::Graphics* graphics, std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms) {
	for (auto transform : transforms) {
		graphics->MultiplyTransform(transform.get());
		draw_one(graphics);
		transform->Invert();
		graphics->MultiplyTransform(transform.get());
		transform->Invert();
	}
}

void Element::draw_original(Gdiplus::Graphics* graphics) {
	draw_one(graphics);
}

std::optional<Gdiplus::RectF> Element::get_bounding_box(std::vector<std::shared_ptr<Element>> elements) {
	std::optional<Gdiplus::RectF> bounds = {};

	for (auto el : elements) {
		auto element_bounds = el->get_bounding_box();
		if (!element_bounds)	// Element is empty
			continue;
		if (!bounds)			// Element is first nonempty element
			bounds = element_bounds;
		else {					// Grow bounds to fit element
			float left = min(bounds->GetLeft(), element_bounds->GetLeft());
			float top = min(bounds->GetTop(), element_bounds->GetTop());
			float right = max(bounds->GetRight(), element_bounds->GetRight());
			float bottom = max(bounds->GetBottom(), element_bounds->GetBottom());

			bounds->X = left;
			bounds->Y = top;
			bounds->Width = right - left;
			bounds->Height = bottom - top;
		}
	}

	return bounds;
}
