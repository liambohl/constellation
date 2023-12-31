#include "ToolEditWallpaperGroup.h"

#include <numbers>

#include "actions/ActionEditWallpaperGroup.h"


const float ToolEditWallpaperGroup::ARROW_HEAD = 0.05f;

ToolEditWallpaperGroup::ToolEditWallpaperGroup(Drawing& drawing, Defaults& defaults) :
    Tool(drawing, defaults)
{
    symmetry_group = std::static_pointer_cast<WallpaperGroup>(drawing.get_symmetry_group());
}

Action* ToolEditWallpaperGroup::handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) {
    Gdiplus::PointF v1 = symmetry_group->get_v1();
    Gdiplus::PointF v2 = symmetry_group->get_v2();

    if (state == IDLE) {
        if (message == WM_LBUTTONDOWN) {
            auto handle = try_select_handle(cursor_pos, scale);
            // Grabbed v1?
            if (handle == "v1") {
                state = DRAGGING_V1;
                vector_initial = v1;
            }
            // Grabbed v2?
            else if (handle == "v2") {
                state = DRAGGING_V2;
                vector_initial = v2;
            }
        }
    }
    else {

        // If control pressed, snap cursor to 15�
        bool control_pressed = key_state & MK_CONTROL;
        if (control_pressed) {
            float r = sqrtf(cursor_pos.X * cursor_pos.X + cursor_pos.Y * cursor_pos.Y);
            float theta = atan2f(cursor_pos.Y, cursor_pos.X);
            theta = roundf(theta * 12 / (float)std::numbers::pi) * (float)std::numbers::pi / 12;    // round theta to the nearest 15�
            cursor_pos = { r * cosf(theta), r * sinf(theta) };
        }

        if (state == DRAGGING_V1) {
            if (message == WM_MOUSEMOVE) {
                symmetry_group->set_v1(cursor_pos);
            }
            if (message == WM_LBUTTONUP) {
                state = IDLE;
                return new ActionEditWallpaperGroup(ActionEditWallpaperGroup::EDIT_V1, vector_initial, cursor_pos);
            }
        }
        else if (state == DRAGGING_V2) {
            if (message == WM_MOUSEMOVE) {
                symmetry_group->set_v2(cursor_pos);
            }
            if (message == WM_LBUTTONUP) {
                state = IDLE;
                return new ActionEditWallpaperGroup(ActionEditWallpaperGroup::EDIT_V2, vector_initial, cursor_pos);
            }
        }
    }
    return nullptr;
}

void ToolEditWallpaperGroup::draw(
    Gdiplus::Graphics* graphics,
    Gdiplus::PointF cursor_pos,
    std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
    float scale
) {
    Gdiplus::PointF v1 = symmetry_group->get_v1();
    Gdiplus::PointF v2 = symmetry_group->get_v2();

    // v1 arrow
    graphics->DrawLine(defaults.symmetry_vector_pen, Gdiplus::PointF(0, 0), v1);
    graphics->DrawLine(defaults.symmetry_vector_pen, v1, { (1 - ARROW_HEAD) * v1.X - ARROW_HEAD * v1.Y, (1 - ARROW_HEAD) * v1.Y + ARROW_HEAD * v1.X });
    graphics->DrawLine(defaults.symmetry_vector_pen, v1, { (1 - ARROW_HEAD) * v1.X + ARROW_HEAD * v1.Y, (1 - ARROW_HEAD) * v1.Y - ARROW_HEAD * v1.X });

    // v2 arrow
    graphics->DrawLine(defaults.symmetry_vector_pen, Gdiplus::PointF(0, 0), v2);
    graphics->DrawLine(defaults.symmetry_vector_pen, v2, { (1 - ARROW_HEAD) * v2.X - ARROW_HEAD * v2.Y, (1 - ARROW_HEAD) * v2.Y + ARROW_HEAD * v2.X });
    graphics->DrawLine(defaults.symmetry_vector_pen, v2, { (1 - ARROW_HEAD) * v2.X + ARROW_HEAD * v2.Y, (1 - ARROW_HEAD) * v2.Y - ARROW_HEAD * v2.X });

    // vector handles
    std::optional<std::string> selected_handle;
    if (state == DRAGGING_V1)
        selected_handle = "v1";
    if (state == DRAGGING_V2)
        selected_handle = "v2";

    draw_handles(graphics, selected_handle, cursor_pos, scale);
}

bool ToolEditWallpaperGroup::update() {
    symmetry_group = dynamic_pointer_cast<WallpaperGroup>(drawing.get_symmetry_group());
    if (symmetry_group == nullptr)
        return true;    // The symmetry group is no longer a wallpaper group, and this tool is irrelevant
    return false;
}

Tool::HandleMap ToolEditWallpaperGroup::get_handles(float scale) {
    Gdiplus::PointF v1 = symmetry_group->get_v1();
    Gdiplus::PointF v2 = symmetry_group->get_v2();

    return {
        { "v1", { HANDLE_CIRCLE, v1 } },
        { "v2", { HANDLE_CIRCLE, v2 } },
    };
}
