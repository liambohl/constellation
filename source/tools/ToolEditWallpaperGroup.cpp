#include "ToolEditWallpaperGroup.h"

#include <numbers>

#include "actions/ActionEditWallpaperGroup.h"

Action* ToolEditWallpaperGroup::handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state, float scale) {
	if (state == INACTIVE) {
		if (message == WM_LBUTTONDOWN) {
			Gdiplus::PointF v1 = symmetry_group->get_v1();
			Gdiplus::PointF v2 = symmetry_group->get_v2();
			// Try to grab v1
			if (powf(x_pos - v1.X, 2.0f) + powf(y_pos - v1.Y, 2.0f) < defaults.selection_range_squared) {
				state = DRAGGING_V1;
				old_x = v1.X;
				old_y = v1.Y;
			}
			// Try to grab v2
			else if (powf(x_pos - v2.X, 2.0f) + powf(y_pos - v2.Y, 2.0f) < defaults.selection_range_squared) {
				state = DRAGGING_V2;
				old_x = v2.X;
				old_y = v2.Y;
			}
		}
	}
	else {
		// If shift pressed, snap cursor to 15�
		bool shift_pressed = key_state & MK_CONTROL;
		if (shift_pressed) {
			float r = sqrtf(x_pos * x_pos + y_pos * y_pos);
			float theta = atan2f(y_pos, x_pos);
			theta = roundf(theta * 12 / (float)std::numbers::pi) * (float)std::numbers::pi / 12;	// round theta to the nearest 15�
			x_pos = r * cosf(theta);
			y_pos = r * sinf(theta);
		}

		if (state == DRAGGING_V1) {
			if (message == WM_MOUSEMOVE) {
				symmetry_group->set_v1(x_pos, y_pos);
			}
			if (message == WM_LBUTTONUP) {
				state = INACTIVE;
				return new ActionEditWallpaperGroup(ActionEditWallpaperGroup::EDIT_V1, old_x, old_y, x_pos, y_pos);
			}
		}
		else if (state == DRAGGING_V2) {
			if (message == WM_MOUSEMOVE) {
				symmetry_group->set_v2(x_pos, y_pos);
			}
			if (message == WM_LBUTTONUP) {
				state = INACTIVE;
				return new ActionEditWallpaperGroup(ActionEditWallpaperGroup::EDIT_V2, old_x, old_y, x_pos, y_pos);
			}
		}
	}
	return nullptr;
}

void ToolEditWallpaperGroup::draw(
	Gdiplus::Graphics* graphics,
	Gdiplus::PointF cursor_pos,
	std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
	float scale
) {
	Gdiplus::PointF v1 = symmetry_group->get_v1();
	Gdiplus::PointF v2 = symmetry_group->get_v2();

	float radius = defaults.handle_radius;

	// v1 arrow
	graphics->DrawLine(defaults.symmetry_vector_pen, Gdiplus::PointF(0, 0), v1);
	graphics->DrawLine(defaults.symmetry_vector_pen, v1, { (1 - ARROW_HEAD) * v1.X - ARROW_HEAD * v1.Y, (1 - ARROW_HEAD) * v1.Y + ARROW_HEAD * v1.X });
	graphics->DrawLine(defaults.symmetry_vector_pen, v1, { (1 - ARROW_HEAD) * v1.X + ARROW_HEAD * v1.Y, (1 - ARROW_HEAD) * v1.Y - ARROW_HEAD * v1.X });
	// v1 handle
	bool hovering_v1 = powf(cursor_pos.X - v1.X, 2.0f) + powf(cursor_pos.Y - v1.Y, 2.0f) < defaults.selection_range_squared;
	bool v1_selected = state == DRAGGING_V1 || hovering_v1;	// hovering over v1 handle
	draw_handle(graphics, v1.X, v1.Y, scale, v1_selected);

	// v2 arrow
	graphics->DrawLine(defaults.symmetry_vector_pen, Gdiplus::PointF(0, 0), v2);
	graphics->DrawLine(defaults.symmetry_vector_pen, v2, { (1 - ARROW_HEAD) * v2.X - ARROW_HEAD * v2.Y, (1 - ARROW_HEAD) * v2.Y + ARROW_HEAD * v2.X });
	graphics->DrawLine(defaults.symmetry_vector_pen, v2, { (1 - ARROW_HEAD) * v2.X + ARROW_HEAD * v2.Y, (1 - ARROW_HEAD) * v2.Y - ARROW_HEAD * v2.X });
	// v2 handle
	bool hovering_v2 = powf(cursor_pos.X - v2.X, 2.0f) + powf(cursor_pos.Y - v2.Y, 2.0f) < defaults.selection_range_squared;
	bool v2_selected = state == DRAGGING_V2 || hovering_v2;	// hovering over v2 handle
	draw_handle(graphics, v2.X, v2.Y, scale, v2_selected);
}
