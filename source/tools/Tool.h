#pragma once

#include <string>
#include <unordered_map>

#include "actions/Action.h"
#include "core/Defaults.h"
#include "drawing/Drawing.h"


class Tool
{
public:
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

	virtual void update() {}	// Check and update this tool's state after doing or undoing an action

	virtual void select_all() {}
	virtual void select_elements(std::vector<std::shared_ptr<Element>> elements) {}	// Select only the given elements

	virtual std::vector<std::shared_ptr<Element>> get_selection() { return {}; }

	virtual Action* handle_delete() { return nullptr; }

	// Attempt to handle an escape event.
	// If this tool is in the middle of an operation, such as drawing a new element, cancel and return true.
	// Otherwise, return false.
	virtual boolean handle_escape() { return false; }

	void set_application_cursor();

	// cursors
	static HCURSOR cursor;					// Current cursor
	static HCURSOR CURSOR_SELECT;			// Default cursor
	static HCURSOR CURSOR_SELECT_HIGHLIGHT;	// Shown when using select tool and hovering over an element
	static HCURSOR CURSOR_PEN;				// Shown when drawing paths

protected:
	struct Handle {
		Gdiplus::Bitmap* default_image;		// image to represent this handle
		Gdiplus::Bitmap* selected_image;	// image if this handle is selected; that is, it is active or the cursor is hovering over it
	};

	// Each handle gets a name, a Handle object (images), and a position.
	typedef std::unordered_map<std::string, std::pair<Handle, Gdiplus::PointF>> HandleMap;

	Tool(Drawing& drawing, Defaults& defaults, HCURSOR cursor = CURSOR_SELECT);

	void set_cursor(HCURSOR cursor) { Tool::cursor = cursor; }

	// Get all handles that should be drawn and selectable.
	virtual HandleMap get_handles(float scale) { return {}; }

	// The name of the closest handle within range of the cursor, if any.
	std::optional<std::string> try_select_handle(Gdiplus::PointF cursor_pos, float scale);

	// Draw all handles. If one is active (being dragged) or close enough to the cursor, draw that handle using its "selected" image.
	void draw_handles(Gdiplus::Graphics* graphics, std::optional<std::string> active_handle, Gdiplus::PointF cursor_pos, float scale);

	// Get the square of the distance between two given points.
	// The points are in world space, but the distance is in page space.
	float distance_squared(Gdiplus::PointF& point_a, Gdiplus::PointF& point_b, float scale);

	Drawing& drawing;
	Defaults& defaults;

	static const float HANDLE_SIZE;				// width AND height, in page space pixels, of handles
	static const float SELECTION_RANGE_SQUARED;	// max distance from center of handle to cursor to allow selection, in page pixels (squared for efficiently comparing distances)

	// images to represent handles
	static Handle RESIZE_TOP_LEFT;
	static Handle RESIZE_TOP_RIGHT;
	static Handle RESIZE_HORIZONTAL;
	static Handle RESIZE_VERTICAL;

	static Handle ROTATE_TOP_LEFT;
	static Handle ROTATE_TOP_RIGHT;
	static Handle ROTATE_BOTTOM_LEFT;
	static Handle ROTATE_BOTTOM_RIGHT;

	static Handle HANDLE_CIRCLE;
	static Handle HANDLE_SQUARE;
	static Handle HANDLE_DIAMOND;
	static Handle HANDLE_MOVE;

	static bool resources_loaded;
};
