#include "ToolEditWallpaperGroup.h"

#include <numbers>

#include "actions/ActionEditWallpaperGroup.h"


const float ToolEditWallpaperGroup::ARROW_HEAD = 0.05f;

Action* ToolEditWallpaperGroup::handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) {
	if (state == IDLE) {
		if (message == WM_LBUTTONDOWN) {
			Gdiplus::PointF v1 = symmetry_group->get_v1();
			Gdiplus::PointF v2 = symmetry_group->get_v2();
			// Try to grab v1
			if (distance_squared(cursor_pos, v1, scale) < SELECTION_RANGE_SQUARED) {
				state = DRAGGING_V1;
				old_x = v1.X;
				old_y = v1.Y;
			}
			// Try to grab v2
			else if (distance_squared(cursor_pos, v2, scale) < SELECTION_RANGE_SQUARED) {
				state = DRAGGING_V2;
				old_x = v2.X;
				old_y = v2.Y;
			}
		}
	}
	else {
		float x_pos = cursor_pos.X;
		float y_pos = cursor_pos.Y;

		// If control pressed, snap cursor to 15°
		bool control_pressed = key_state & MK_CONTROL;
		if (control_pressed) {
			float r = sqrtf(x_pos * x_pos + y_pos * y_pos);
			float theta = atan2f(y_pos, x_pos);
			theta = roundf(theta * 12 / (float)std::numbers::pi) * (float)std::numbers::pi / 12;	// round theta to the nearest 15°
			x_pos = r * cosf(theta);
			y_pos = r * sinf(theta);
		}

		if (state == DRAGGING_V1) {
			if (message == WM_MOUSEMOVE) {
				symmetry_group->set_v1(x_pos, y_pos);
			}
			if (message == WM_LBUTTONUP) {
				state = IDLE;
				return new ActionEditWallpaperGroup(ActionEditWallpaperGroup::EDIT_V1, old_x, old_y, x_pos, y_pos);
			}
		}
		else if (state == DRAGGING_V2) {
			if (message == WM_MOUSEMOVE) {
				symmetry_group->set_v2(x_pos, y_pos);
			}
			if (message == WM_LBUTTONUP) {
				state = IDLE;
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

	// v1 arrow
	graphics->DrawLine(defaults.symmetry_vector_pen, Gdiplus::PointF(0, 0), v1);
	graphics->DrawLine(defaults.symmetry_vector_pen, v1, { (1 - ARROW_HEAD) * v1.X - ARROW_HEAD * v1.Y, (1 - ARROW_HEAD) * v1.Y + ARROW_HEAD * v1.X });
	graphics->DrawLine(defaults.symmetry_vector_pen, v1, { (1 - ARROW_HEAD) * v1.X + ARROW_HEAD * v1.Y, (1 - ARROW_HEAD) * v1.Y - ARROW_HEAD * v1.X });

	// v2 arrow
	graphics->DrawLine(defaults.symmetry_vector_pen, Gdiplus::PointF(0, 0), v2);
	graphics->DrawLine(defaults.symmetry_vector_pen, v2, { (1 - ARROW_HEAD) * v2.X - ARROW_HEAD * v2.Y, (1 - ARROW_HEAD) * v2.Y + ARROW_HEAD * v2.X });
	graphics->DrawLine(defaults.symmetry_vector_pen, v2, { (1 - ARROW_HEAD) * v2.X + ARROW_HEAD * v2.Y, (1 - ARROW_HEAD) * v2.Y - ARROW_HEAD * v2.X });

	// vector handles
	Handle v1_handle = { handle_circle, handle_circle_selected, v1 };
	Handle v2_handle = { handle_circle, handle_circle_selected, v2 };

	std::optional<Handle> selected_handle;
	if (state == DRAGGING_V1)
		selected_handle = v1_handle;
	if (state == DRAGGING_V2)
		selected_handle = v2_handle;

	draw_handles(graphics, { v1_handle, v2_handle }, selected_handle, cursor_pos, scale);
}
