#pragma once
#include "Tool.h"
#include "Path.h"

namespace Constellation {

	class ToolNewPath :
	    public Tool
	{
	private:
		// Which mouse action are we waiting for?
		enum BezierState {
			FIRST_DOWN,		// n_nodes = 0
			FIRST_RELEASE,	// n_nodes = 0
			SECOND_DOWN,	// n_nodes = 1
			NTH_RELEASE,	// n_nodes >= 1
			NTH_DOWN		// n_nodes >= 2. We can only finish a path if we are in this state.
		};

		Path* wip_path = nullptr;	// The "work in progress" path we will soon add to the drawing
		Path* tool_path = nullptr;	// The last segment of path which we are editing and may or may not make it into the drawing
		BezierState state = FIRST_DOWN;
		int n_nodes = 0;			// number of nodes in wip_path
		int next_node_x = 0;
		int next_node_y = 0;

	public:
		ToolNewPath(Drawing& drawing) : Tool(drawing) { }

		Action* handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam);
		void draw(Canvas& canvas);

		boolean undo() override;

		boolean handle_escape() override;
	};
	
}
