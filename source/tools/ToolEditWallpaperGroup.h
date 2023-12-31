#pragma once

#include "Tool.h"

#include "drawing/symmetry/WallpaperGroup.h"


class ToolEditWallpaperGroup :
    public Tool
{
public:
    ToolEditWallpaperGroup(Drawing& drawing, Defaults& defaults);

    Action* handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) override;

    void draw(
        Gdiplus::Graphics* graphics,
        Gdiplus::PointF cursor_pos,
        std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
        float scale
    ) override;

    bool update() override;

private:
    HandleMap get_handles(float scale) override;

    // length of an arrow head as a proportion of the length of an arrow
    static const float ARROW_HEAD;

    enum State {
        IDLE,
        DRAGGING_V1,
        DRAGGING_V2
    };

    State state = IDLE;

    std::shared_ptr<WallpaperGroup> symmetry_group;

    Gdiplus::PointF vector_initial; // Initial value of v1 or v2 before we started moving it
};

