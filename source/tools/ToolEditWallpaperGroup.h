#pragma once

#include "Tool.h"

#include "drawing/symmetry/WallpaperGroup.h"


class ToolEditWallpaperGroup :
    public Tool
{
public:
	ToolEditWallpaperGroup(Drawing& drawing, Defaults& defaults) :
		Tool(drawing, defaults)
	{
		symmetry_group = std::static_pointer_cast<WallpaperGroup>(drawing.get_symmetry_group());
	}

	Action* handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) override;

	void draw(
		Gdiplus::Graphics* graphics,
		Gdiplus::PointF cursor_pos,
		std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
		float scale
	) override;

private:
	// length of an arrow head as a proportion of the length of an arrow
	static const float ARROW_HEAD;

	enum State {
		IDLE,
		DRAGGING_V1,
		DRAGGING_V2
	};

	State state = IDLE;

	std::shared_ptr<WallpaperGroup> symmetry_group;

	float old_x = 0.0f, old_y = 0.0f;
};

