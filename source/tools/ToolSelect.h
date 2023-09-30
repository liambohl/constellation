#pragma once

#include "Tool.h"

#include <vector>

#include "drawing/Group.h"


class ToolSelect :
    public Tool
{
public:
    ToolSelect(Drawing& drawing, Defaults& defaults) : Tool(drawing, defaults) { }

    Action* handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state, float scale) override;

    void draw(
		Gdiplus::Graphics* graphics,
		Gdiplus::PointF cursor_pos,
		std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
		float scale
	) override;

private:
	enum State {
		RESIZE,	// dragging handles resizes selected elements
		ROTATE	// dragging handles rotates selected elements around center point
	};

	enum Handle {
		NONE,			// no handle is selected
		TOP_LEFT,		// top left handle
		TOP,
		TOP_RIGHT,
		RIGHT,
		BOTTOM_RIGHT,
		BOTTOM,
		BOTTOM_LEFT,
		LEFT,
		CENTER,			// rotation center
		SELECTION_BOX	// selecting an area
	};

	void add_or_remove_element(std::shared_ptr<Element> element);	// If the selection contains the element, remove it. Otherwise, add it.

	State state = RESIZE;
	Handle selected_handle = NONE;

	std::vector<std::shared_ptr<Element>> selection;
};
