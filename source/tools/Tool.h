#pragma once

#include "actions/Action.h"
#include "core/Defaults.h"
#include "drawing/Drawing.h"


class Tool
{
public:
	~Tool();

	// If an action is completed, return a pointer to that action. Else, return nullptr.
	virtual Action* handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) = 0;

	virtual void draw(
		Gdiplus::Graphics* graphics,
		Gdiplus::PointF cursor_pos,
		std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
		float scale
	) = 0;
		
	// Attempt to handle an undo/redo event.
	// If this tool can perform an undo/redo, such as while drawing a long path, do so and return true.
	virtual boolean undo() { return false; }
	virtual boolean redo() { return false; }

	virtual void select_all() {}

	// Attempt to handle an escape event.
	// If this tool is in the middle of an operation, such as drawing a new element, cancel and return true.
	// Otherwise, return false.
	virtual boolean handle_escape() { return false; }

protected:
	//typedef std::pair<Gdiplus::Bitmap*, Gdiplus::Bitmap*> Handle;
	struct Handle {
		Gdiplus::Bitmap* image;				// image to represent this handle
		Gdiplus::Bitmap* selected_image;	// image if this handle is selected; that is, it is active or the cursor is hovering over it
		Gdiplus::PointF position;			// location of top-left corner
	};

	Tool(Drawing& drawing, Defaults& defaults);

	// Get the square of the distance between two given points.
	// The points are in world space, but the distance is in page space.
	float distance_squared(Gdiplus::PointF& point_a, Gdiplus::PointF& point_b, float scale);

	// Draw all handles. If one is active (being dragged) or close enough to the cursor, draw that handle using its "selected" image.
	void draw_handles(Gdiplus::Graphics* graphics, std::vector<Handle> handles, std::optional<Handle> active_handle, Gdiplus::PointF cursor_pos, float scale);

	Drawing& drawing;
	Defaults& defaults;

	static const float HANDLE_SIZE;				// width AND height, in page space pixels, of handles
	static const float SELECTION_RANGE_SQUARED;	// max distance from center of handle to cursor to allow selection, in page pixels (squared for efficiently comparing distances)

	// images to represent handles
	Gdiplus::Bitmap* resize_top_left;
	Gdiplus::Bitmap* resize_top_left_selected;
	Gdiplus::Bitmap* resize_top_right;
	Gdiplus::Bitmap* resize_top_right_selected;
	Gdiplus::Bitmap* resize_horizontal;
	Gdiplus::Bitmap* resize_horizontal_selected;
	Gdiplus::Bitmap* resize_vertical;
	Gdiplus::Bitmap* resize_vertical_selected;

	Gdiplus::Bitmap* rotate_top_left;
	Gdiplus::Bitmap* rotate_top_left_selected;
	Gdiplus::Bitmap* rotate_top_right;
	Gdiplus::Bitmap* rotate_top_right_selected;
	Gdiplus::Bitmap* rotate_bottom_left;
	Gdiplus::Bitmap* rotate_bottom_left_selected;
	Gdiplus::Bitmap* rotate_bottom_right;
	Gdiplus::Bitmap* rotate_bottom_right_selected;

	Gdiplus::Bitmap* handle_circle;
	Gdiplus::Bitmap* handle_circle_selected;
	Gdiplus::Bitmap* handle_square;
	Gdiplus::Bitmap* handle_square_selected;
	Gdiplus::Bitmap* handle_diamond;
	Gdiplus::Bitmap* handle_diamond_selected;
	Gdiplus::Bitmap* handle_move;
	Gdiplus::Bitmap* handle_move_selected;

};
