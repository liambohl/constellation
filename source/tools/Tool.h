#pragma once

#include "actions/Action.h"
#include "core/Defaults.h"


class Tool
{
public:
	// If an action is completed, return a pointer to that action. Else, return nullptr.
	virtual Action* handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state, float scale) = 0;

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

	// Attempt to handle an escape event.
	// If this tool is in the middle of an operation, such as drawing a new element, cancel and return true.
	// Otherwise, return false.
	virtual boolean handle_escape() { return false; }

protected:
	Tool(Defaults& defaults) : defaults(defaults) {}

	void draw_handle(Gdiplus::Graphics* graphics, float x, float y, float scale, bool selected = false);

	Defaults& defaults;
};
