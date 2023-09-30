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
