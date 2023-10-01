#include "Tool.h"

#include "core/resource.h"
#include "helpers/image.h"


const float Tool::HANDLE_SIZE = 15.0f;
const float Tool::SELECTION_RANGE_SQUARED = 100.0f;

Tool::~Tool() {
	delete resize_top_left;
	delete resize_top_left_selected;
	delete resize_top_right;
	delete resize_top_right_selected;
	delete resize_horizontal;
	delete resize_horizontal_selected;
	delete resize_vertical;
	delete resize_vertical_selected;

	delete rotate_top_left;
	delete rotate_top_left_selected;
	delete rotate_top_right;
	delete rotate_top_right_selected;
	delete rotate_bottom_left;
	delete rotate_bottom_left_selected;
	delete rotate_bottom_right;
	delete rotate_bottom_right_selected;

	delete handle_circle;
	delete handle_circle_selected;
	delete handle_square;
	delete handle_square_selected;
	delete handle_diamond;
	delete handle_diamond_selected;
	delete handle_move;
	delete handle_move_selected;
}

Tool::Tool(Drawing& drawing, Defaults& defaults) : drawing(drawing), defaults(defaults) {
	resize_top_left              = image_from_resource(IDB_RESIZE_TOP_LEFT, L"PNG");
	resize_top_left_selected     = image_from_resource(IDB_RESIZE_TOP_LEFT_SELECTED, L"PNG");
	resize_top_right             = image_from_resource(IDB_RESIZE_TOP_RIGHT, L"PNG");
	resize_top_right_selected    = image_from_resource(IDB_RESIZE_TOP_RIGHT_SELECTED, L"PNG");
	resize_horizontal            = image_from_resource(IDB_RESIZE_HORIZONTAL, L"PNG");
	resize_horizontal_selected   = image_from_resource(IDB_RESIZE_HORIZONTAL_SELECTED, L"PNG");
	resize_vertical              = image_from_resource(IDB_RESIZE_VERTICAL, L"PNG");
	resize_vertical_selected     = image_from_resource(IDB_RESIZE_VERTICAL_SELECTED, L"PNG");

	rotate_top_left              = image_from_resource(IDB_ROTATE_TOP_LEFT, L"PNG");
	rotate_top_left_selected     = image_from_resource(IDB_ROTATE_TOP_LEFT_SELECTED, L"PNG");
	rotate_top_right             = image_from_resource(IDB_ROTATE_TOP_RIGHT, L"PNG");
	rotate_top_right_selected    = image_from_resource(IDB_ROTATE_TOP_RIGHT_SELECTED, L"PNG");
	rotate_bottom_left           = image_from_resource(IDB_ROTATE_BOTTOM_LEFT, L"PNG");
	rotate_bottom_left_selected  = image_from_resource(IDB_ROTATE_BOTTOM_LEFT_SELECTED, L"PNG");
	rotate_bottom_right          = image_from_resource(IDB_ROTATE_BOTTOM_RIGHT, L"PNG");
	rotate_bottom_right_selected = image_from_resource(IDB_ROTATE_BOTTOM_RIGHT_SELECTED, L"PNG");
	
	handle_circle                = image_from_resource(IDB_HANDLE_CIRCLE, L"PNG");
	handle_circle_selected       = image_from_resource(IDB_HANDLE_CIRCLE_SELECTED, L"PNG");
	handle_square                = image_from_resource(IDB_HANDLE_SQUARE, L"PNG");
	handle_square_selected       = image_from_resource(IDB_HANDLE_SQUARE_SELECTED, L"PNG");
	handle_diamond               = image_from_resource(IDB_HANDLE_DIAMOND, L"PNG");
	handle_diamond_selected      = image_from_resource(IDB_HANDLE_DIAMOND_SELECTED, L"PNG");
	handle_move                  = image_from_resource(IDB_HANDLE_MOVE, L"PNG");
	handle_move_selected         = image_from_resource(IDB_HANDLE_MOVE_SELECTED, L"PNG");
}

float Tool::distance_squared(Gdiplus::PointF& point_a, Gdiplus::PointF& point_b, float scale) {
	return powf((point_a.X - point_b.X) * scale, 2.0f) + powf((point_a.Y - point_b.Y) * scale, 2.0f);
}

void Tool::draw_handles(Gdiplus::Graphics* graphics, std::vector<Handle> handles, std::optional<Handle> active_handle, Gdiplus::PointF cursor_pos, float scale) {
	std::optional<Handle> selected_handle;	// The handle, if any, to draw with its selected image

	float SIZE = HANDLE_SIZE / scale;
	
	if (active_handle) {
		selected_handle = active_handle;
	}
	else {
		// Select the closest handle within range of the cursor, if any.
		float closest_distance_squared = (std::numeric_limits<float>::max)();	// parentheses prevent the max macro from expanding

		for (auto handle : handles) {
			// distance from cursor to center of handle (squared)
			float handle_distance_squared = distance_squared(cursor_pos, handle.position, scale);

			if (handle_distance_squared < SELECTION_RANGE_SQUARED) {
				if (handle_distance_squared < closest_distance_squared) {
					closest_distance_squared = handle_distance_squared;
					selected_handle = handle;
				}
			}
		}
	}

	// Draw each handle with its normal image
	for (auto handle : handles) {
		graphics->DrawImage(handle.image, handle.position.X - SIZE / 2, handle.position.Y - SIZE / 2, SIZE, SIZE);
	}

	// Draw selected handle on top
	if (selected_handle) {
		graphics->DrawImage(selected_handle->selected_image, selected_handle->position.X - SIZE / 2, selected_handle->position.Y - SIZE / 2, SIZE, SIZE);
	}
}
