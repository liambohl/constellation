#pragma once
#include "Tool.h"
#include "Path.h"

namespace Constellation {

	class BezierTool :
	    public Tool
	{
	private:
		// Which mouse action are we waiting for?
		enum BezierState {
			FIRST_DOWN,
			FIRST_RELEASE,
			SECOND_DOWN,
			SECOND_RELEASE,
			NTH_DOWN,		// We can only finish a Bezier curve from this state
			NTH_RELEASE
		};

		Path* wip_path = nullptr;	// The "work in progress" path we will soon add to the drawing
		Path* tool_path = nullptr;	// The last segment of path which we are editing and may or may not make it into the drawing
		BezierState state = FIRST_DOWN;
		int next_node_x = 0;
		int next_node_y = 0;

	public:
		BezierTool(Drawing& drawing) : Tool(drawing) { }

		Action* handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);
		void draw(Canvas& canvas);
	};
	
}
