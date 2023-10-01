#include "ToolSelect.h"


const float ToolSelect::SELECTION_MARGIN = 5.0f;

Action* ToolSelect::handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) {
	// If anything selected, give handles a chance to handle mouse event
	// Otherwise,
	if (message == WM_LBUTTONDOWN) {
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
				else
					selection = { element };
			}
		}
		else if (!shift_pressed) {	// Click off to deselect all and return to resize mode
			selection.clear();
			mode = RESIZE;
		}
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

	// If selection contains any stuff, draw handles
	if (bounds) {
		const float SIZE = HANDLE_SIZE / scale;
		const float TOP = bounds->GetTop();
		const float BOTTOM = bounds->GetBottom();
		const float LEFT = bounds->GetLeft();
		const float RIGHT = bounds->GetRight();

		std::vector<Handle> handles;
		
		if (mode == RESIZE) {
			handles = {
				{ resize_top_left, resize_top_left_selected, { LEFT - SIZE / 2, TOP - SIZE / 2 } },
				{ resize_vertical, resize_vertical_selected, { (LEFT + RIGHT) / 2, TOP - SIZE / 2 } },
				{ resize_top_right, resize_top_right_selected, { RIGHT + SIZE / 2, TOP - SIZE / 2 } },
				{ resize_horizontal, resize_horizontal_selected, { RIGHT + SIZE / 2, (TOP + BOTTOM) / 2 } },
				{ resize_top_left, resize_top_left_selected, { RIGHT + SIZE / 2, BOTTOM + SIZE / 2 } },
				{ resize_vertical, resize_vertical_selected, { (LEFT + RIGHT) / 2, BOTTOM + SIZE / 2 } },
				{ resize_top_right, resize_top_right_selected, { LEFT - SIZE / 2, BOTTOM + SIZE / 2 } },
				{ resize_horizontal, resize_horizontal_selected, { LEFT - SIZE / 2, (TOP + BOTTOM) / 2 } }
			};
		}
		else {
			handles = {
				{ rotate_top_left, rotate_top_left_selected, { LEFT - SIZE / 2, TOP - SIZE / 2 } },
				{ rotate_top_right, rotate_top_right_selected, { RIGHT + SIZE / 2, TOP - SIZE / 2 } },
				{ rotate_bottom_right, rotate_bottom_right_selected, { RIGHT + SIZE / 2, BOTTOM + SIZE / 2 } },
				{ rotate_bottom_left, rotate_bottom_left_selected, { LEFT - SIZE / 2, BOTTOM + SIZE / 2 } }
			};
		}

		draw_handles(graphics, handles, {}, cursor_pos, scale);
	}
}

void ToolSelect::add_or_remove_element(std::shared_ptr<Element> element) {
	auto iter = std::find(selection.begin(), selection.end(), element);
	if (iter != selection.end())
		selection.erase(iter);
	else
		selection.push_back(element);
}
