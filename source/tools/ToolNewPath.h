#pragma once

#include "Tool.h"

#include "drawing/Path.h"


class ToolNewPath :
	public Tool
{
public:
	ToolNewPath(Defaults& defaults) : Tool(defaults) { }

	Action* handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state, float scale) override;

	void draw(
		Gdiplus::Graphics* graphics,
		Gdiplus::PointF cursor_pos,
		std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
		float scale
	) override;

	boolean undo() override;

	boolean handle_escape() override;

private:
	// Which mouse action are we waiting for?
	enum BezierState {
		FIRST_DOWN,		// n_nodes = 0
		FIRST_RELEASE,	// n_nodes = 0
		SECOND_DOWN,	// n_nodes = 1
		NTH_RELEASE,	// n_nodes >= 1
		NTH_DOWN		// n_nodes >= 2. We can only finish a path if we are in this state.
	};

	std::vector<Gdiplus::PointF> wip_path;	// The "work in progress" path we will soon add to the drawing
	std::vector<Gdiplus::PointF> tool_path;	// The last segment of path which we are editing and may or may not make it into the drawing

	BezierState state = FIRST_DOWN;

	int n_nodes = 0;			// number of nodes in wip_path

	float next_node_x = 0;
	float next_node_y = 0;
};
