#pragma once
#include "Tool.h"


class ToolEditPathNodes :
    public Tool
{
public:
	ToolEditPathNodes(Drawing& drawing, Defaults& defaults) : Tool(drawing, defaults, CURSOR_NODES) { }

	Action* handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) override;

	void draw(
		Gdiplus::Graphics* graphics,
		Gdiplus::PointF cursor_pos,
		std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
		float scale
	) override;
};

