#include "ToolSelect.h"


const float ToolSelect::SELECTION_MARGIN = 5.0f;

Action* ToolSelect::handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) {
	// If anything selected, give handles a chance to handle mouse event
	// Otherwise,
	if (message == WM_LBUTTONDOWN) {
		// Try to select a handle
		active_handle = try_select_handle(cursor_pos, scale);

		if (active_handle.has_value()) {
			return nullptr;
		}

		bool shift_pressed = key_state & MK_SHIFT;

		std::shared_ptr<Element> element = drawing.select_element(cursor_pos, SELECTION_MARGIN, scale);

		// If we clicked an element
		if (element != nullptr) {
			if (shift_pressed)
				add_or_remove_element(element);
			else {
				// If we click on an element in the selection, switch modes
				if (std::find(selection.begin(), selection.end(), element) != selection.end())
					mode = (mode == RESIZE) ? ROTATE : RESIZE;
				// If we clicked a different element, select only it
				else {
					selection = { element };
					update_bounds();
				}
			}
		}
		else if (!shift_pressed) {	// Click off to deselect all and return to resize mode
			selection.clear();
			update_bounds();
			mode = RESIZE;
		}
	}
	else if (message == WM_LBUTTONUP) {
		active_handle = {};
	}
	return nullptr;
}

void ToolSelect::draw(
	Gdiplus::Graphics* graphics,
	Gdiplus::PointF cursor_pos,
	std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
	float scale
) {
	for (auto& el : selection) {
		auto element_bounds = el->get_bounding_box();
		if (!element_bounds)	// Element is empty
			continue;

		// Draw bounding box around selected element
		graphics->DrawRectangle(defaults.selection_pen_1, *element_bounds);
		graphics->DrawRectangle(defaults.selection_pen_2, *element_bounds);
	}

	// If selection contains any stuff, draw handles
	if (bounds) {
		draw_handles(graphics, active_handle, cursor_pos, scale);
	}
}

void ToolSelect::select_all() {
	selection = drawing.select_all();
	update_bounds();
}

// Escape to clear selection, if any
boolean ToolSelect::handle_escape() {
	if (selection.empty())
		return false;
	selection.clear();
	update_bounds();
	return true;
}

void ToolSelect::add_or_remove_element(std::shared_ptr<Element> element) {
	auto iter = std::find(selection.begin(), selection.end(), element);
	if (iter != selection.end())
		selection.erase(iter);
	else
		selection.push_back(element);
	update_bounds();
}

void ToolSelect::update_bounds() {
	bounds = {};

	for (auto el : selection) {
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
}

Tool::HandleMap ToolSelect::get_handles(float scale) {
	if (bounds) {
		const float SIZE = HANDLE_SIZE / scale;
		const float TOP = bounds->GetTop();
		const float BOTTOM = bounds->GetBottom();
		const float LEFT = bounds->GetLeft();
		const float RIGHT = bounds->GetRight();

		if (mode == RESIZE) {
			return {
				{ "resize_top_left",     { RESIZE_TOP_LEFT,     { LEFT - SIZE / 2,    TOP - SIZE / 2     } } },
				{ "resize_top",          { RESIZE_VERTICAL,     { (LEFT + RIGHT) / 2, TOP - SIZE / 2     } } },
				{ "resize_top_right",    { RESIZE_TOP_RIGHT,    { RIGHT + SIZE / 2,   TOP - SIZE / 2     } } },
				{ "resize_right",        { RESIZE_HORIZONTAL,   { RIGHT + SIZE / 2,   (TOP + BOTTOM) / 2 } } },
				{ "resize_bottom_right", { RESIZE_TOP_LEFT,     { RIGHT + SIZE / 2,   BOTTOM + SIZE / 2  } } },
				{ "resize_bottom",       { RESIZE_VERTICAL,     { (LEFT + RIGHT) / 2, BOTTOM + SIZE / 2  } } },
				{ "resize_bottom_left",  { RESIZE_TOP_RIGHT,    { LEFT - SIZE / 2,    BOTTOM + SIZE / 2  } } },
				{ "resize_left",         { RESIZE_HORIZONTAL,   { LEFT - SIZE / 2,    (TOP + BOTTOM) / 2 } } }
			};
		}
		else {
			return {
				{ "rotate_top_left",     { ROTATE_TOP_LEFT,     { LEFT - SIZE / 2,    TOP - SIZE / 2     } } },
				{ "rotate_top_right",    { ROTATE_TOP_RIGHT,    { RIGHT + SIZE / 2,   TOP - SIZE / 2     } } },
				{ "rotate_bottom_right", { ROTATE_BOTTOM_RIGHT, { RIGHT + SIZE / 2,   BOTTOM + SIZE / 2  } } },
				{ "rotate_bottom_left",  { ROTATE_BOTTOM_LEFT,  { LEFT - SIZE / 2,    BOTTOM + SIZE / 2  } } }
			};
		}
	}
	else
		return {};	// No selection; no handles
}
