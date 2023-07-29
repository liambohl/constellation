#pragma once
#include "Action.h"
#include "Defaults.h"

class Tool
{
protected:
	Defaults& defaults;

	Tool(Defaults& defaults) : defaults(defaults) {}

public:
	// If an action is completed, return a pointer to that action. Else, return nullptr.
	virtual Action* handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state) = 0;
	virtual void draw(Gdiplus::Graphics* graphics) = 0;
		
	// Attempt to handle an undo/redo event.
	// If this tool can perform an undo/redo, such as while drawing a long path, do so and return true.
	virtual boolean undo() { return false; }
	virtual boolean redo() { return false; }

	// Attempt to handle an escape event.
	// If this tool is in the middle of an operation, such as drawing a new element, cancel and return true.
	// Otherwise, return false.
	virtual boolean handle_escape() { return false; }
};
