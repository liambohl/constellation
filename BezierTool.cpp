#include <windowsx.h>

#include "BezierTool.h"
#include "Element.h"
#include "Logger.h"

namespace Constellation {

	void BezierTool::handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam) {
		int x_pos = GET_X_LPARAM(lParam);
		int y_pos = GET_Y_LPARAM(lParam);
		
		if (message != WM_MOUSEMOVE)
			*Logger::get_instance() << '(' << x_pos << ", " << y_pos << "), state = " << state << std::endl;

		switch (state) {
		case FIRST_DOWN:
			if (message == WM_LBUTTONDOWN) {
				wip_path = new Path(drawing.defaults.wip_pen->Clone());
				wip_path->add_point(x_pos, y_pos);

				// For now, the temp path is a line segment. As we move the cursor, the 3rd and 4th points will move with it.
				// Only the first point is set in stone.
				tool_path = new Path(drawing.defaults.tool_pen->Clone());
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);

				state = FIRST_RELEASE;
			}
			break;
			
		case FIRST_RELEASE:
			if (message == WM_MOUSEMOVE) {
				tool_path->pop_point();
				tool_path->pop_point();
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
			}
			else if (message == WM_LBUTTONUP) {
				//path->add_point(x_pos, y_pos);
				// The second point (the first handle) is now set in stone.
				tool_path->pop_point();
				tool_path->pop_point();
				tool_path->pop_point();
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
				state = SECOND_DOWN;
			}
			break;

		case SECOND_DOWN:
			if (message == WM_MOUSEMOVE) {
				tool_path->pop_point();
				tool_path->pop_point();
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
			}
			else if (message == WM_LBUTTONDOWN) {
				next_node_x = x_pos;
				next_node_y = y_pos;
				state = SECOND_RELEASE;
			}
			break;

		case SECOND_RELEASE:
		case NTH_RELEASE:
		{
			int trailing_handle_x = 2 * next_node_x - x_pos;
			int trailing_handle_y = 2 * next_node_y - y_pos;
			tool_path->pop_point();
			tool_path->pop_point();
			tool_path->add_point(trailing_handle_x, trailing_handle_y);	// trailing handle
			tool_path->add_point(next_node_x, next_node_y);				// node

			if (message == WM_LBUTTONUP) {
				wip_path->add_points(*tool_path);
				// For now, the temp path is a line segment. As we move the cursor, the 3rd and 4th points will move with it.
				// The first two points are set in stone.
				delete tool_path;
				tool_path = new Path(drawing.defaults.tool_pen->Clone());
				tool_path->add_point(next_node_x, next_node_y);
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
				state = NTH_DOWN;
			}
			break;
		}

		case NTH_DOWN:
			if (message == WM_MOUSEMOVE) {
				tool_path->pop_point();
				tool_path->pop_point();
				tool_path->add_point(x_pos, y_pos);
				tool_path->add_point(x_pos, y_pos);
			}
			else if (message == WM_LBUTTONDOWN) {
				next_node_x = x_pos;
				next_node_y = y_pos;
				state = NTH_RELEASE;
			}
			else if (message == WM_RBUTTONDOWN) {
				wip_path->pen = drawing.defaults.pen->Clone();
				drawing.elements.push_back(wip_path);
				state = FIRST_DOWN;
			}
			break;

		}
	}

	void BezierTool::draw(Canvas& canvas) {
		if (wip_path != nullptr)
			wip_path->draw(canvas);
		if (state != FIRST_DOWN)
			tool_path->draw(canvas);
	}

}
