#include <windowsx.h>

#include "actions/ActionAddElement.h"
#include "ToolNewPath.h"
#include "drawing/Element.h"
#include "core/Logger.h"

Action* ToolNewPath::handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state) {
	if (message != WM_MOUSEMOVE)
		*Logger::get_instance() << '(' << x_pos << ", " << y_pos << "), state = " << state << std::endl;

	switch (state) {
	case FIRST_DOWN:
		if (message == WM_LBUTTONDOWN) {
			wip_path = new Path(defaults.wip_pen->Clone());
			wip_path->add_point(x_pos, y_pos);

			// For now, the temp path is a line segment. As we move the cursor, the 3rd and 4th points will move with it.
			// Only the first point is set in stone.
			tool_path = new Path(defaults.tool_pen->Clone());
			tool_path->add_point(x_pos, y_pos, 4);
			state = FIRST_RELEASE;
		}
		break;
			
	case FIRST_RELEASE:
		if (message == WM_MOUSEMOVE) {
			tool_path->pop_point(2);
			tool_path->add_point(x_pos, y_pos, 2);
		}
		else if (message == WM_LBUTTONUP) {
			//path->add_point(x_pos, y_pos);
			// The second point (the first handle) is now set in stone.
			tool_path->pop_point(3);
			tool_path->add_point(x_pos, y_pos, 3);
			state = SECOND_DOWN;
			n_nodes = 1;
		}
		break;

	case SECOND_DOWN:
		if (message == WM_MOUSEMOVE) {
			tool_path->pop_point(2);
			tool_path->add_point(x_pos, y_pos, 2);
		}
		else if (message == WM_LBUTTONDOWN) {
			next_node_x = x_pos;
			next_node_y = y_pos;
			state = NTH_RELEASE;
		}
		break;

	case NTH_RELEASE:
	{
		float trailing_handle_x = 2 * next_node_x - x_pos;
		float trailing_handle_y = 2 * next_node_y - y_pos;
		tool_path->pop_point(2);
		tool_path->add_point(trailing_handle_x, trailing_handle_y);	// trailing handle
		tool_path->add_point(next_node_x, next_node_y);				// node

		if (message == WM_LBUTTONUP) {
			wip_path->add_points(*tool_path);
			// For now, the temp path is a line segment. As we move the cursor, the 3rd and 4th points will move with it.
			// The first two points are set in stone.
			delete tool_path;
			tool_path = new Path(defaults.tool_pen->Clone());
			tool_path->add_point(next_node_x, next_node_y);
			tool_path->add_point(x_pos, y_pos, 3);
			state = NTH_DOWN;
			++n_nodes;
		}
		break;
	}

	case NTH_DOWN:
		if (message == WM_MOUSEMOVE) {
			tool_path->pop_point(2);
			tool_path->add_point(x_pos, y_pos, 2);
		}
		else if (message == WM_LBUTTONDOWN) {
			next_node_x = x_pos;
			next_node_y = y_pos;
			state = NTH_RELEASE;
		}
		else if (message == WM_RBUTTONDOWN) {
			wip_path->pen = defaults.pen->Clone();
			state = FIRST_DOWN;
			// Hand over wip_path to the drawing
			Path* final_path = wip_path;
			wip_path = nullptr;
			return new ActionAddElement(std::shared_ptr<Element>(final_path));
		}
		break;

	}
		
	return nullptr;
}

void ToolNewPath::draw(Gdiplus::Graphics* graphics) {
	if (state != FIRST_DOWN) {
		wip_path->draw(graphics);
		tool_path->draw(graphics);
	}
}

boolean ToolNewPath::undo() {
	switch (state) {
	case FIRST_DOWN:
		return false;	// nothing to undo
	case FIRST_RELEASE:
	case SECOND_DOWN:
		delete wip_path;
		state = FIRST_DOWN;
		n_nodes = 0;
		return true;
	case NTH_RELEASE: {
		Gdiplus::PointF node = tool_path->top();
		tool_path->pop_point();
		Gdiplus::PointF trailing_handle = tool_path->top();
		tool_path->pop_point();
		float handle_x = 2 * node.X - trailing_handle.X;
		float handle_y = 2 * node.Y - trailing_handle.Y;
		tool_path->add_point(handle_x, handle_y, 2);
		if (n_nodes == 1)
			state = SECOND_DOWN;
		else
			state = NTH_DOWN;
		return true;
	}
	case NTH_DOWN: {
		wip_path->pop_point(2);
		Gdiplus::PointF p1 = wip_path->top();
		wip_path->pop_point();
		Gdiplus::PointF p0 = wip_path->top();

		Gdiplus::PointF p3 = tool_path->top();
		tool_path->pop_point();
		Gdiplus::PointF p2 = tool_path->top();

		tool_path = new Path(defaults.tool_pen->Clone());
		tool_path->add_point(p0.X, p0.Y);
		tool_path->add_point(p1.X, p1.Y);
		tool_path->add_point(p2.X, p2.Y);
		tool_path->add_point(p3.X, p3.Y);

		--n_nodes;
		if (n_nodes == 1)
			state = SECOND_DOWN;
		return true;
	}
	default:	// This should never execute, since all possible values of state are handled
		return false;
	}
}

boolean ToolNewPath::handle_escape() {
	if (state == FIRST_DOWN) {
		return false;
	}
	else {
		// Cancel the in-progress path;
		delete wip_path;
		delete tool_path;
		state = FIRST_DOWN;
		return true;
	}
}
