#include "ToolSelect.h"

#include "actions/ActionRemoveElements.h"
#include "actions/ActionTransformElements.h"


const float ToolSelect::SELECTION_MARGIN = 5.0f;

Action* ToolSelect::handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) {
	bool shift_pressed = key_state & MK_SHIFT;
	bool control_pressed = key_state & MK_CONTROL;

	switch (state) {
	case IDLE:
		if (message == WM_LBUTTONDOWN) {
			// Try to select a handle
			active_handle = try_select_handle(cursor_pos, scale);
			if (active_handle) {
				state = DRAGGING_HANDLE;
				click_position = cursor_pos;
				last_drag_position = cursor_pos; 
				return nullptr;
			}

			// Try to select an element
			std::shared_ptr<Element> element = drawing.select_element(cursor_pos, SELECTION_MARGIN, scale);
			if (element != nullptr) {
				if (shift_pressed)
					add_or_remove_element(element);
				else {
					// If we click on an element outside the selection, select only it
					if (std::find(selection.begin(), selection.end(), element) == selection.end()) {
						selection = { element };
						new_selection = true;
						mode = RESIZE;
						update_bounds();
					}

					// Enable dragging the selection
					state = DRAGGING_SELECTION;
					click_position = cursor_pos;
					last_drag_position = cursor_pos;
				}
			}
			else if (!shift_pressed) {	// Click off to deselect all
				deselect_all();
			}
		}
		break;
	case DRAGGING_HANDLE:
		if (message == WM_LBUTTONUP) {
			active_handle = {};
			state = IDLE;
		}
		break;
	case DRAGGING_SELECTION:
		if (message == WM_LBUTTONUP) {
			state = IDLE;
			if (cursor_pos.Equals(click_position)) {
				// When we click on an element in the selection (without dragging), switch modes
				if (!new_selection)
					mode = (mode == RESIZE) ? ROTATE : RESIZE;
			}
			else {
				// After dragging the selection, undo temporary translation and commit an Action
				auto temp_delta = click_position - last_drag_position;
				for (const auto& element : selection) {
					Gdiplus::Matrix* temp_matrix = new Gdiplus::Matrix(1.0f, 0, 0, 1.0f, temp_delta.X, temp_delta.Y);
					element->transform(temp_matrix);
				}
				auto overall_delta = cursor_pos - click_position;
				Gdiplus::Matrix* overall_matrix = new Gdiplus::Matrix(1.0f, 0, 0, 1.0f, overall_delta.X, overall_delta.Y);
				return new ActionTransformElements(selection, overall_matrix);
			}
			new_selection = false;
		}
		else if (message == WM_MOUSEMOVE) {
			auto delta = cursor_pos - last_drag_position;	// Translation since we handled the last message
			Gdiplus::Matrix translation_matrix(1.0f, 0, 0, 1.0f, delta.X, delta.Y);
			for (const auto& element : selection)
				element->transform(&translation_matrix);
			last_drag_position = cursor_pos;
			update_bounds();
		}
		break;
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

void ToolSelect::update() {
	// Remove any elements from the selection which are no longer in the drawing.
	const auto& extant_elements = drawing.get_elements();
	// std::remove_if moves these elements to the end of selection, then selection.erase deletes them.
	selection.erase(
		std::remove_if(
			selection.begin(),
			selection.end(),
			[&extant_elements](const std::shared_ptr<Element>& selected_element) {
				return std::find(extant_elements.begin(), extant_elements.end(), selected_element) == extant_elements.end();
			}
		),
		selection.end()
	);

	update_bounds();
}

void ToolSelect::select_all() {
	selection = drawing.get_elements();
	update_bounds();
}

void ToolSelect::select_elements(std::vector<std::shared_ptr<Element>> elements) {
	selection = elements;
	update_bounds();
}

Action* ToolSelect::handle_delete() {
	if (!selection.empty()) {
		return new ActionRemoveElements(selection);
	}
	return nullptr;
}

// Escape to clear selection, if any
boolean ToolSelect::handle_escape() {
	if (selection.empty())
		return false;
	deselect_all();
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

void ToolSelect::deselect_all() {
	selection.clear();
	update_bounds();
	mode = RESIZE;
}

void ToolSelect::update_bounds() {
	bounds = Element::get_bounding_box(selection);
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
