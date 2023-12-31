#include "ToolSelect.h"

#include <numbers>

#include "actions/ActionRemoveElements.h"
#include "actions/ActionTransformElements.h"


const float ToolSelect::SELECTION_MARGIN = 5.0f;

Action* ToolSelect::handle_mouse_event(UINT message, Gdiplus::PointF cursor_pos, int key_state, float scale) {
    bool shift_pressed = key_state & MK_SHIFT;
    bool control_pressed = key_state & MK_CONTROL;

    switch (state) {
    case IDLE:
        if (message == WM_LBUTTONDOWN) {
            click_position = cursor_pos;

            // Try to select a handle
            active_handle = try_select_handle(cursor_pos, scale);
            if (active_handle) {
                state = DRAGGING_HANDLE;
                initial_bounds = Element::get_bounding_box(selection, false);
                set_click_offset();
                return nullptr;
            }

            // Try to select an element
            std::shared_ptr<Element> element = drawing.select_element(cursor_pos, SELECTION_MARGIN, scale);
            if (element != nullptr) {
                if (shift_pressed)
                    add_or_remove_element(element);
                else {
                    // If we click on an element outside the selection, select only it
                    if (std::find(selection.begin(), selection.end(), element) == selection.end()) {
                        selection = { element };
                        new_selection = true;
                        mode = RESIZE;
                        update_bounds();
                        update_rotation_center();
                    }

                    // Enable dragging the selection
                    state = DRAGGING_SELECTION;
                }
            }

            // Start a rectangular selection area
            else {
                state = SELECTING_AREA;
            }
        }
        else if (message == WM_MOUSEMOVE) {
            // If an element is in selection range, indicate that by highlighting the cursor
            std::shared_ptr<Element> element = drawing.select_element(cursor_pos, SELECTION_MARGIN, scale);
            if (element != nullptr)
                set_cursor(CURSOR_SELECT_HIGHLIGHT);
            else
                set_cursor(CURSOR_SELECT);
        }
        break;
    case DRAGGING_HANDLE:
        if (message == WM_LBUTTONUP) {
            Gdiplus::Matrix* transform = new Gdiplus::Matrix;
            // Rotate selection
            if (mode == ROTATE) {
                if (active_handle == "rotation_center") {
                    rotation_center = cursor_pos;
                }
                else {
                    rotate_transform(cursor_pos, transform, shift_pressed, control_pressed);
                    update_bounds();
                }
            }
            // Resize selection
            else {
                resize_transform(cursor_pos, transform, shift_pressed, control_pressed);
                update_bounds();
                update_rotation_center();
            }

            active_handle = {};
            state = IDLE;
            return new ActionTransformElements(selection, transform);
        }
        else if (message == WM_MOUSEMOVE) {
            Gdiplus::Matrix* transform = new Gdiplus::Matrix;
            // Temporarily rotate selection
            if (mode == ROTATE) {
                if (active_handle == "rotation_center") {
                    rotation_center = cursor_pos;
                }
                else {
                    rotate_transform(cursor_pos, transform, shift_pressed, control_pressed);
                    update_bounds();
                }
            }
            // Temporarily resize selection
            else {
                resize_transform(cursor_pos, transform, shift_pressed, control_pressed);
                update_bounds();
                update_rotation_center();
            }
            delete transform;
        }
        break;
    case DRAGGING_SELECTION:
        if (message == WM_LBUTTONUP) {
            state = IDLE;
            if (cursor_pos.Equals(click_position)) {
                // When we click on an element in the selection (without dragging), switch modes
                if (!new_selection)
                    mode = (mode == RESIZE) ? ROTATE : RESIZE;
            }
            else {
                // After dragging the selection, commit the translation as an Action
                auto delta = cursor_pos - click_position;
                Gdiplus::Matrix* transform = new Gdiplus::Matrix;
                transform->Translate(delta.X, delta.Y);
                return new ActionTransformElements(selection, transform);
            }
            new_selection = false;
        }
        else if (message == WM_MOUSEMOVE) {
            auto delta = cursor_pos - click_position;
            Gdiplus::Matrix* transform = new Gdiplus::Matrix;
            transform->Translate(delta.X, delta.Y);
            for (const auto& element : selection)
                element->transform_temp(transform);
            update_bounds();
            update_rotation_center();
        }
        break;
    case SELECTING_AREA:
        // Select everything in the selection area
        if (message == WM_LBUTTONUP) {
            if (!shift_pressed) {
                selection = {};
                new_selection = true;
                mode = RESIZE;
            }

            float left = min(click_position.X, cursor_pos.X);
            float right = max(click_position.X, cursor_pos.X);
            float top = min(click_position.Y, cursor_pos.Y);
            float bottom = max(click_position.Y, cursor_pos.Y);

            Gdiplus::RectF selection_area(left, top, right - left, bottom - top);

            // If rectangle is drawn left-to-right, select everything completely contained by the rectangle.
            if (cursor_pos.X > click_position.X) {
                for (auto& element : drawing.get_elements()) {
                    auto bounding_box = element->get_bounding_box();
                    if (
                        bounding_box->GetLeft() > left &&
                        bounding_box->GetRight() < right &&
                        bounding_box->GetTop() > top &&
                        bounding_box->GetBottom() < bottom
                    ) {
                        selection.push_back(element);
                    }
                }
            }

            // If rectangle is drawn right-to-left, select everything that overlaps the rectangle.
            else {
                for (auto& element : drawing.get_elements()) {
                    auto bounding_box = element->get_bounding_box();
                    if (element->intersects_rectangle(selection_area)) {
                        selection.push_back(element);
                    }
                }
            }

            state = IDLE;
            update_bounds();
            update_rotation_center();
        }
    }
    return nullptr;
}

void ToolSelect::draw(
    Gdiplus::Graphics* graphics,
    Gdiplus::PointF cursor_pos,
    std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
    float scale
) {
    for (auto& el : selection) {
        auto element_bounds = el->get_bounding_box();
        if (!element_bounds)    // Element is empty
            continue;

        // Draw bounding box around selected element
        graphics->DrawRectangle(defaults.selection_pen_1, *element_bounds);
        graphics->DrawRectangle(defaults.selection_pen_2, *element_bounds);
    }

    // If selection contains any stuff, draw handles
    if (bounds) {
        draw_handles(graphics, active_handle, cursor_pos, scale);
    }

    // Draw selection rectangle
    if (state == SELECTING_AREA) {
        float left = min(click_position.X, cursor_pos.X);
        float right = max(click_position.X, cursor_pos.X);
        float top = min(click_position.Y, cursor_pos.Y);
        float bottom = max(click_position.Y, cursor_pos.Y);

        Gdiplus::RectF selection_area(left, top, right - left, bottom - top);

        if (click_position.X < cursor_pos.X) {
            graphics->FillRectangle(defaults.ltr_selection_rectangle_brush, selection_area);
            graphics->DrawRectangle(defaults.ltr_selection_rectangle_pen, selection_area);
        }
        else {
            graphics->FillRectangle(defaults.rtl_selection_rectangle_brush, selection_area);
            graphics->DrawRectangle(defaults.rtl_selection_rectangle_pen, selection_area);
        }
    }
}

bool ToolSelect::update() {
    // Remove any elements from the selection which are no longer in the drawing.
    const auto& extant_elements = drawing.get_elements();
    // std::remove_if moves these elements to the end of selection, then selection.erase deletes them.
    selection.erase(
        std::remove_if(
            selection.begin(),
            selection.end(),
            [&extant_elements](const std::shared_ptr<Element>& selected_element) {
                return std::find(extant_elements.begin(), extant_elements.end(), selected_element) == extant_elements.end();
            }
        ),
        selection.end()
    );

    update_bounds();
    return false;   // Tool is still okay
}

void ToolSelect::select_all() {
    selection = drawing.get_elements();
    update_bounds();
    update_rotation_center();
}

void ToolSelect::select_elements(std::vector<std::shared_ptr<Element>> elements) {
    selection = elements;
    update_bounds();
    update_rotation_center();
}

Action* ToolSelect::handle_delete() {
    if (!selection.empty()) {
        return new ActionRemoveElements(selection);
    }
    return nullptr;
}

// Escape to clear selection, if any
boolean ToolSelect::handle_escape() {
    if (selection.empty())
        return false;
    deselect_all();
    return true;
}

void ToolSelect::add_or_remove_element(std::shared_ptr<Element> element) {
    auto iter = std::find(selection.begin(), selection.end(), element);
    if (iter != selection.end())
        selection.erase(iter);
    else
        selection.push_back(element);
    update_bounds();
    update_rotation_center();
}

void ToolSelect::deselect_all() {
    selection.clear();
    update_bounds();
    update_rotation_center();
    mode = RESIZE;
}

void ToolSelect::update_bounds() {
    bounds = Element::get_bounding_box(selection);
}

void ToolSelect::update_rotation_center() {
    if (bounds)
        rotation_center = { (bounds->GetLeft() + bounds->GetRight()) / 2, (bounds->GetTop() + bounds->GetBottom()) / 2 };
}

void ToolSelect::set_click_offset() {
    if (mode == RESIZE) {
        if (active_handle == "resize_top_left") {
            click_offset = click_position - Gdiplus::PointF(initial_bounds->GetLeft(), initial_bounds->GetTop());
        }
        else if (active_handle == "resize_top") {
            click_offset = click_position - Gdiplus::PointF((initial_bounds->GetLeft() + initial_bounds->GetRight()) / 2.0f, initial_bounds->GetTop());
        }
        else if (active_handle == "resize_top_right") {
            click_offset = click_position - Gdiplus::PointF(initial_bounds->GetRight(), initial_bounds->GetTop());
        }
        else if (active_handle == "resize_right") {
            click_offset = click_position - Gdiplus::PointF(initial_bounds->GetRight(), (initial_bounds->GetTop() + initial_bounds->GetBottom()) / 2.0f);
        }
        else if (active_handle == "resize_bottom_right") {
            click_offset = click_position - Gdiplus::PointF(initial_bounds->GetRight(), initial_bounds->GetBottom());
        }
        else if (active_handle == "resize_bottom") {
            click_offset = click_position - Gdiplus::PointF((initial_bounds->GetLeft() + initial_bounds->GetRight()) / 2.0f, initial_bounds->GetBottom());
        }
        else if (active_handle == "resize_bottom_left") {
            click_offset = click_position - Gdiplus::PointF(initial_bounds->GetLeft(), initial_bounds->GetBottom());
        }
        else if (active_handle == "resize_left") {
            click_offset = click_position - Gdiplus::PointF(initial_bounds->GetLeft(), (initial_bounds->GetTop() + initial_bounds->GetBottom()) / 2.0f);
        }
    }
}

void ToolSelect::rotate_transform(Gdiplus::PointF& cursor_pos, Gdiplus::Matrix* transform, bool shift_pressed, bool control_pressed) {
    // In radians
    float theta_initial = atan2f(click_position.Y - rotation_center.Y, click_position.X - rotation_center.X);
    float theta = atan2f(cursor_pos.Y - rotation_center.Y, cursor_pos.X - rotation_center.X);
    // In degrees
    float delta_theta = (theta - theta_initial) * 180.0f / (float)std::numbers::pi;
    // Press control to snap to 15-degree increments
    if (control_pressed)
        delta_theta = 15.0f * roundf(delta_theta / 15.0f);

    transform->RotateAt(delta_theta, rotation_center);

    // Apply transform
    for (const auto& element : selection)
        element->transform_temp(transform);
}

void ToolSelect::resize_transform(Gdiplus::PointF& cursor_pos, Gdiplus::Matrix* transform, bool shift_pressed, bool control_pressed) {
    float scale_x = 1.0f;
    float scale_y = 1.0f;
    // The anchor point stays put during the transform. If shift is pressed, anchor is the center of the selection.
    // Otherwise, anchor is the point on initial_bounds opposite the active handle.
    Gdiplus::PointF anchor = {
        (initial_bounds->GetLeft() + initial_bounds->GetRight()) / 2.0f,
        (initial_bounds->GetTop() + initial_bounds->GetBottom()) / 2.0f
    };

    if (active_handle == "resize_top_left" || active_handle == "resize_top" || active_handle == "resize_top_right") {
        scale_y = -(cursor_pos.Y - click_offset.Y - initial_bounds->GetBottom()) / initial_bounds->Height;
        anchor.Y = initial_bounds->GetBottom();
    }
    else if (active_handle == "resize_bottom_left" || active_handle == "resize_bottom" || active_handle == "resize_bottom_right") {
        scale_y = (cursor_pos.Y - click_offset.Y - initial_bounds->GetTop()) / initial_bounds->Height;
        anchor.Y = initial_bounds->GetTop();
    }

    if (active_handle == "resize_top_left" || active_handle == "resize_left" || active_handle == "resize_bottom_left") {
        scale_x = -(cursor_pos.X - click_offset.X - initial_bounds->GetRight()) / initial_bounds->Width;
        anchor.X = initial_bounds->GetRight();
    }
    else if (active_handle == "resize_top_right" || active_handle == "resize_right" || active_handle == "resize_bottom_right") {
        scale_x = (cursor_pos.X - click_offset.X - initial_bounds->GetLeft()) / initial_bounds->Width;
        anchor.X = initial_bounds->GetLeft();
    }

    // Press control to scale both x and y equally
    if (control_pressed) {
        float scale = 1.0f;
        if (active_handle == "resize_top_left" || active_handle == "resize_top_right" ||
            active_handle == "resize_bottom_right" || active_handle == "resize_bottom_left")
            scale = (scale_x + scale_y) / 2.0f;
        else if (active_handle == "resize_top" || active_handle == "resize_bottom")
            scale = scale_y;
        else if (active_handle == "resize_left" || active_handle == "resize_right")
            scale = scale_x;

        scale_x = scale;
        scale_y = scale;
    }

    // Press shift to resize about the center of the selection.
    if (shift_pressed) {
        // Double the change in width and height
        scale_x = 2 * scale_x - 1;
        scale_y = 2 * scale_y - 1;

        anchor = {
            (initial_bounds->GetLeft() + initial_bounds->GetRight()) / 2.0f,
            (initial_bounds->GetTop() + initial_bounds->GetBottom()) / 2.0f
        };
    }

    transform->Translate(anchor.X * (1 - scale_x), anchor.Y * (1 - scale_y));
    transform->Scale(scale_x, scale_y);

    // Apply transform
    for (const auto& element : selection)
        element->transform_temp(transform);
}

Tool::HandleMap ToolSelect::get_handles(float scale) {
    if (bounds) {
        const float SIZE = HANDLE_SIZE / scale;
        const float TOP = bounds->GetTop();
        const float BOTTOM = bounds->GetBottom();
        const float LEFT = bounds->GetLeft();
        const float RIGHT = bounds->GetRight();
        const float CENTER_X = rotation_center.X;
        const float CENTER_Y = rotation_center.Y;

        if (mode == RESIZE) {
            return {
                { "resize_top_left",     { RESIZE_TOP_LEFT,     { LEFT - SIZE / 2,    TOP - SIZE / 2     } } },
                { "resize_top",          { RESIZE_VERTICAL,     { (LEFT + RIGHT) / 2, TOP - SIZE / 2     } } },
                { "resize_top_right",    { RESIZE_TOP_RIGHT,    { RIGHT + SIZE / 2,   TOP - SIZE / 2     } } },
                { "resize_right",        { RESIZE_HORIZONTAL,   { RIGHT + SIZE / 2,   (TOP + BOTTOM) / 2 } } },
                { "resize_bottom_right", { RESIZE_TOP_LEFT,     { RIGHT + SIZE / 2,   BOTTOM + SIZE / 2  } } },
                { "resize_bottom",       { RESIZE_VERTICAL,     { (LEFT + RIGHT) / 2, BOTTOM + SIZE / 2  } } },
                { "resize_bottom_left",  { RESIZE_TOP_RIGHT,    { LEFT - SIZE / 2,    BOTTOM + SIZE / 2  } } },
                { "resize_left",         { RESIZE_HORIZONTAL,   { LEFT - SIZE / 2,    (TOP + BOTTOM) / 2 } } }
            };
        }
        else {
            return {
                { "rotate_top_left",     { ROTATE_TOP_LEFT,     { LEFT - SIZE / 2,    TOP - SIZE / 2     } } },
                { "rotate_top_right",    { ROTATE_TOP_RIGHT,    { RIGHT + SIZE / 2,   TOP - SIZE / 2     } } },
                { "rotate_bottom_right", { ROTATE_BOTTOM_RIGHT, { RIGHT + SIZE / 2,   BOTTOM + SIZE / 2  } } },
                { "rotate_bottom_left",  { ROTATE_BOTTOM_LEFT,  { LEFT - SIZE / 2,    BOTTOM + SIZE / 2  } } },
                { "rotation_center",     { HANDLE_CIRCLE,       { CENTER_X,           CENTER_Y           } } }
            };
        }
    }
    else
        return {};    // No selection; no handles
}
