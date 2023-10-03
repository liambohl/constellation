#pragma once

#include "Tool.h"

#include <vector>

#include "drawing/Group.h"


class ToolSelect :
    public Tool
{
public:
    ToolSelect(Drawing& drawing, Defaults& defaults) : Tool(drawing, defaults) { }

    Action* handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) override;

    void draw(
		Gdiplus::Graphics* graphics,
		Gdiplus::PointF cursor_pos,
		std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
		float scale
	) override;

	void select_all() override;

	boolean handle_escape() override;

private:
	enum Mode {
		RESIZE,	// dragging handles resizes selected elements
		ROTATE	// dragging handles rotates selected elements around center point
	};


	void add_or_remove_element(std::shared_ptr<Element> element);	// If the selection contains the element, remove it. Otherwise, add it.

	void update_bounds();

	HandleMap get_handles(float scale) override;

	Mode mode = RESIZE;

	std::optional<std::string> active_handle;

	std::vector<std::shared_ptr<Element>> selection;
	
	std::optional<Gdiplus::RectF> bounds;

	static const float SELECTION_MARGIN;
};
