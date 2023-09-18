#pragma once
#include "Tool.h"

class ToolSelect :
    public Tool
{
public:
    ToolSelect(Defaults& defaults) : Tool(defaults) { }

    Action* handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state, float scale) override;

    void draw(
		Gdiplus::Graphics* graphics,
		Gdiplus::PointF cursor_pos,
		std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
		float scale
	) override;
};
