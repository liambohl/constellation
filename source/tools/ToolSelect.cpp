#include "ToolSelect.h"


Action* ToolSelect::handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state, float scale) {
	// If anything selected, give handles a chance to handle mouse event
	// Otherwise,
	if (message == WM_LBUTTONDOWN) {
		bool shift_pressed = key_state & MK_SHIFT;

		Gdiplus::PointF cursor_pos(x_pos, y_pos);
		std::shared_ptr<Element> element = drawing.select_element(cursor_pos);

		// If we clicked an element
		if (element != nullptr) {
			if (shift_pressed)
				add_or_remove_element(element);
			else
				selection = { element };
		}
		else if (!shift_pressed)	// Click off to deselect all
			selection.clear();
	}
	return nullptr;
}

void ToolSelect::draw(
	Gdiplus::Graphics* graphics,
	Gdiplus::PointF cursor_pos,
	std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
	float scale
) {
	std::optional<Gdiplus::RectF> bounds;

	for (auto el : selection) {
		auto element_bounds = el->get_bounding_box();
		if (!element_bounds)	// Element is empty
			continue;

		// Draw bounding box around selected element
		graphics->DrawRectangle(defaults.selection_pen_1, *element_bounds);
		graphics->DrawRectangle(defaults.selection_pen_2, *element_bounds);

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

	if (bounds) {
		graphics->DrawRectangle(defaults.selection_pen_1, *bounds);
		graphics->DrawRectangle(defaults.selection_pen_2, *bounds);
	}
}

void ToolSelect::add_or_remove_element(std::shared_ptr<Element> element) {
	auto iter = std::find(selection.begin(), selection.end(), element);
	if (iter != selection.end())
		selection.erase(iter);
	else
		selection.push_back(element);
}