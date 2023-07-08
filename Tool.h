#pragma once
#include "Action.h"
#include "Canvas.h"
#include "Drawing.h"

namespace Constellation {
		
	class Tool
	{
	protected:
		Drawing& drawing;

		Tool(Drawing& drawing) : drawing(drawing) {}

	public:
		// If an action is completed, return a pointer to that action. Else, return nullptr.
		virtual Action* handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam) = 0;
		virtual void draw(Canvas& canvas) = 0;
		
		// Attempt to handle an undo/redo event.
		// If this tool can perform an undo/redo, such as while drawing a long path, do so and return true.
		boolean undo() { return false; }
		boolean redo() { return false; }
	};

}
