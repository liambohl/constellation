#include "ToolNewPath.h"

#include <windowsx.h>

#include "actions/ActionAddElements.h"
#include "drawing/Element.h"
#include "core/Logger.h"


Action* ToolNewPath::handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) {
	if (message != WM_MOUSEMOVE)
		*Logger::get_instance() << '(' << cursor_pos.X << ", " << cursor_pos.Y << "), state = " << state << std::endl;

	switch (state) {
	case FIRST_DOWN:	// tool_path and wip_path are both empty
		if (message == WM_LBUTTONDOWN) {
			wip_path.clear();
			wip_path.push_back(cursor_pos);

			// For now, the temp path is a line segment. As we move the cursor, the 3rd and 4th points will move with it.
			// Only the first point is set in stone.
			tool_path.clear();
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
			state = FIRST_RELEASE;
		}
		break;
			
	case FIRST_RELEASE:	// tool_path is a line. wip_path is empty.
		if (message == WM_MOUSEMOVE) {
			tool_path.pop_back();
			tool_path.pop_back();
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
		}
		else if (message == WM_LBUTTONUP) {
			// The second point (the first handle) is now set in stone.
			tool_path.pop_back();
			tool_path.pop_back();
			tool_path.pop_back();
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
			state = SECOND_DOWN;
			n_nodes = 1;
		}
		break;

	case SECOND_DOWN:	// tool_path is a curve. wip_path is empty.
		if (message == WM_MOUSEMOVE) {
			tool_path.pop_back();
			tool_path.pop_back();
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
		}
		else if (message == WM_LBUTTONDOWN) {
			next_node_x = cursor_pos.X;
			next_node_y = cursor_pos.Y;
			state = NTH_RELEASE;
		}
		break;

	case NTH_RELEASE:	// tool_path is a curve. wip_path is a sequence of zero or more Bezier curves.
	{
		float trailing_handle_x = 2 * next_node_x - cursor_pos.X;
		float trailing_handle_y = 2 * next_node_y - cursor_pos.Y;
		tool_path.pop_back();
		tool_path.pop_back();
		tool_path.push_back({ trailing_handle_x, trailing_handle_y });	// trailing handle
		tool_path.push_back({ next_node_x, next_node_y });				// node

		if (message == WM_LBUTTONUP) {
			std::copy(tool_path.begin() + 1, tool_path.end(), std::back_inserter(wip_path));
			// For now, tool_path is a line segment. As we move the cursor, the 3rd and 4th points will move with it.
			// The first two points are set in stone.
			tool_path.clear();
			tool_path.push_back({ next_node_x, next_node_y });
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
			state = NTH_DOWN;
			++n_nodes;
			auto& logger = *Logger::get_instance();
			logger << "wip_path: { ";
			for (Gdiplus::PointF& point : wip_path)
				logger << "{ " << point.X << ", " << point.Y << " }, ";
			logger << "}" << std::endl;
		}
		break;
	}

	case NTH_DOWN:		// tool_path is a curve. wip_path is a sequence of one or more Bezier curves.
		if (message == WM_MOUSEMOVE) {
			tool_path.pop_back();
			tool_path.pop_back();
			tool_path.push_back(cursor_pos);
			tool_path.push_back(cursor_pos);
		}
		else if (message == WM_LBUTTONDOWN) {
			next_node_x = cursor_pos.X;
			next_node_y = cursor_pos.Y;
			state = NTH_RELEASE;
		}
		else if (message == WM_RBUTTONDOWN) {
			state = FIRST_DOWN;
			// Hand over wip_path to the drawing
			auto new_path = std::make_shared<Path>(defaults.pen);
			new_path->add_beziers(wip_path);
			return new ActionAddElements({ new_path });
		}
		break;

	}

	return nullptr;
}

void ToolNewPath::draw(
	Gdiplus::Graphics* graphics,
	Gdiplus::PointF cursor_pos,
	std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
	float scale
) {
	if (state != FIRST_DOWN) {
		for (auto& transform : transforms) {
			graphics->MultiplyTransform(transform.get());
			
			// Draw path in progress
			graphics->DrawBeziers(defaults.wip_pen, wip_path.data(), (int)wip_path.size());

			// Draw tangent line segment at the last node
			if (state == NTH_RELEASE)
				graphics->DrawLine(defaults.tangent_pen, cursor_pos, tool_path[2]);
			else if (state == NTH_DOWN)
				graphics->DrawLine(defaults.tangent_pen, tool_path[1], wip_path[wip_path.size() - 2]);

			// Draw new path segment
			graphics->DrawBeziers(defaults.tool_pen, tool_path.data(), (int)tool_path.size());

			transform->Invert();
			graphics->MultiplyTransform(transform.get());
			transform->Invert();
		}
	}
}

boolean ToolNewPath::undo() {
	switch (state) {
	case FIRST_DOWN:
		return false;	// nothing to undo
	case FIRST_RELEASE:
	case SECOND_DOWN:
		wip_path.clear();
		state = FIRST_DOWN;
		n_nodes = 0;
		return true;
	case NTH_RELEASE: {
		Gdiplus::PointF node = tool_path.back();
		tool_path.pop_back();
		Gdiplus::PointF trailing_handle = tool_path.back();
		tool_path.pop_back();
		float handle_x = 2 * node.X - trailing_handle.X;
		float handle_y = 2 * node.Y - trailing_handle.Y;
		tool_path.push_back({ handle_x, handle_y });
		tool_path.push_back({ handle_x, handle_y });
		if (n_nodes == 1)
			state = SECOND_DOWN;
		else
			state = NTH_DOWN;
		return true;
	}
	case NTH_DOWN: {
		wip_path.pop_back();
		wip_path.pop_back();
		Gdiplus::PointF p1 = wip_path.back();
		wip_path.pop_back();
		Gdiplus::PointF p0 = wip_path.back();

		Gdiplus::PointF p3 = tool_path.back();
		tool_path.pop_back();
		Gdiplus::PointF p2 = tool_path.back();

		tool_path.clear();
		tool_path.push_back({ p0.X, p0.Y });
		tool_path.push_back({ p1.X, p1.Y });
		tool_path.push_back({ p2.X, p2.Y });
		tool_path.push_back({ p3.X, p3.Y });

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
		wip_path.clear();
		tool_path.clear();
		state = FIRST_DOWN;
		return true;
	}
}
