#include <fstream>
#include <thread>

#include <windowsx.h>

#include "ConstellationApp.h"
#include "Logger.h"
#include "actions/ActionChangeSymmetryGroup.h"
#include "actions/ActionAddElements.h"
#include "actions/ActionRemoveElements.h"
#include "drawing/symmetry/SymmetryGroupFactory.h"
#include "tools/ToolEditWallpaperGroup.h"
#include "tools/ToolNewPath.h"
#include "tools/ToolSelect.h"


ConstellationApp::ConstellationApp(HWND hWnd) : drawing(hWnd)
{
    set_tool(SELECT);

    // Get screen refresh rate and calculate refresh interval
    DEVMODEA* dm = new DEVMODEA();
    dm->dmSize = sizeof(DEVMODEA);
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, dm);
    int refresh_rate = dm->dmDisplayFrequency;
    if (refresh_rate < 59)
        refresh_rate = 60;
    refresh_interval_ns = 1000000000 / refresh_rate;
    delete dm;
    *Logger::get_instance() << "refresh interval: " << refresh_interval_ns << std::endl;
}

void ConstellationApp::open_file(HWND hWnd, LPWSTR file_path) {
    drawing_file_path = file_path;
    open_file(hWnd);
}

void ConstellationApp::new_drawing(HWND hWnd) {
    drawing_file_path = nullptr;
    drawing_folder = nullptr;
    drawing = Drawing(hWnd);
    canvas.reset_transform();
    reset_unsaved_changes();
    reset_history();
    set_tool(SELECT);

    *Logger::get_instance() << "New file" << std::endl;
}

void ConstellationApp::open(HWND hWnd) {
    if (open_cst_file(&drawing_file_path, drawing_folder)) {
        open_file(hWnd);
        *Logger::get_instance() << "Opened file \"" << drawing_file_path << "\"" << std::endl;
    }
}

void ConstellationApp::save() {
    // If working on new file, choose save location.
    if (drawing_file_path == nullptr) {
        save_as_cst_file(&drawing_file_path, drawing_folder);
    }

    // If save location has been chosen, save.
    if (drawing_file_path != nullptr) {
        save_file();
        *Logger::get_instance() << "Saved file \"" << drawing_file_path << "\"" << std::endl;
    }
}

void ConstellationApp::save_as() {
    if (save_as_cst_file(&drawing_file_path, drawing_folder)) {
        save_file();
        *Logger::get_instance() << "Saved file as \"" << drawing_file_path << "\"" << std::endl;
    }
}

void ConstellationApp::undo() {
    if (current_tool->undo())
        return;

    if (undo_stack.size() > 0) {
        Action* last = undo_stack.top();
        last->undo(drawing);
        undo_stack.pop();
        redo_stack.push(last);

        // Tell current tool to reflect changes. If it is no longer relevant, revert to select tool
        if (current_tool->update())
            set_tool(SELECT);
        --unsaved_changes;
    }
}

void ConstellationApp::redo() {
    if (redo_stack.size() > 0) {
        Action* next = redo_stack.top();
        next->apply(drawing);
        redo_stack.pop();
        undo_stack.push(next);

        if (current_tool->update())
            set_tool(SELECT);
        ++unsaved_changes;
    }
}

void ConstellationApp::cut_to_clipboard() {
    std::vector<std::shared_ptr<Element>> cut_elements = current_tool->get_selection();
    if (!cut_elements.empty()) {
        clipboard = cut_elements;
        do_action(new ActionRemoveElements(cut_elements));
    }
}

void ConstellationApp::copy_to_clipboard() {
    std::vector<std::shared_ptr<Element>> copied_elements = current_tool->get_selection();
    if (!copied_elements.empty())
        clipboard = copied_elements;
}

void ConstellationApp::paste_from_clipboard(HWND hWnd) {
    if (!clipboard.empty()) {
        auto pasted_elements = clone_elements(clipboard);
        auto bounds = Element::get_bounding_box(pasted_elements);
        if (bounds) {
            Gdiplus::PointF bounds_center((bounds->GetLeft() + bounds->GetRight()) / 2, (bounds->GetTop() + bounds->GetBottom()) / 2);
            Gdiplus::PointF cursor_pos = get_cursor_pos(hWnd);
            Gdiplus::PointF delta = cursor_pos - bounds_center; // how far, and in what direction, we should move the elements to center them at the cursor position
            Gdiplus::Matrix translation_matrix(1.0f, 0, 0, 1.0f, delta.X, delta.Y);
            for (auto& element : pasted_elements)
                element->transform(&translation_matrix);
        }
        do_action(new ActionAddElements(pasted_elements));
        current_tool->select_elements(pasted_elements);
    }
}

void ConstellationApp::duplicate() {
    std::vector<std::shared_ptr<Element>> selected_elements = current_tool->get_selection();
    if (!selected_elements.empty()) {
        auto duplicate_elements = clone_elements(selected_elements);
        do_action(new ActionAddElements(duplicate_elements));
        current_tool->select_elements(duplicate_elements);
    }
}

void ConstellationApp::handle_delete() {
    Action* action = current_tool->handle_delete();
    if (action != nullptr)
        do_action(action);
}

void ConstellationApp::select_all() {
    current_tool->select_all();
}

void ConstellationApp::set_tool(enum tool tool_type) {
    delete current_tool;
    switch (tool_type) {
    case NEW_PATH:
        current_tool = new ToolNewPath(drawing, defaults);
        break;
    case SELECT:
        current_tool = new ToolSelect(drawing, defaults);
        break;
    case EDIT_SYMMETRY_PARAMETERS:
        current_tool = new ToolEditWallpaperGroup(drawing, defaults);
        break;
    }
}

void ConstellationApp::set_symmetry_group(enum symmetry_group symmetry_group) {
    std::shared_ptr<SymmetryGroup> old_group = drawing.get_symmetry_group();
    std::shared_ptr<SymmetryGroup> new_group;

    switch (symmetry_group) {
    case TRIVIAL:
        new_group = SymmetryGroupFactory::get_instance().trivial();
        break;
    case P1:
        new_group = SymmetryGroupFactory::get_instance().p1(old_group);
        break;
    case P2:
        new_group = SymmetryGroupFactory::get_instance().p2(old_group);
        break;
    case PM:
        new_group = SymmetryGroupFactory::get_instance().pm(old_group);
        break;
    case PG:
        new_group = SymmetryGroupFactory::get_instance().pg(old_group);
        break;
    case CM:
        new_group = SymmetryGroupFactory::get_instance().cm(old_group);
        break;
    case PMM:
        new_group = SymmetryGroupFactory::get_instance().pmm(old_group);
        break;
    case PMG:
        new_group = SymmetryGroupFactory::get_instance().pmg(old_group);
        break;
    case PGG:
        new_group = SymmetryGroupFactory::get_instance().pgg(old_group);
        break;
    case CMM:
        new_group = SymmetryGroupFactory::get_instance().cmm(old_group);
        break;
    case P4:
        new_group = SymmetryGroupFactory::get_instance().p4(old_group);
        break;
    case P4M:
        new_group = SymmetryGroupFactory::get_instance().p4m(old_group);
        break;
    case P4G:
        new_group = SymmetryGroupFactory::get_instance().p4g(old_group);
        break;
    case P3:
        new_group = SymmetryGroupFactory::get_instance().p3(old_group);
        break;
    case P3M1:
        new_group = SymmetryGroupFactory::get_instance().p3m1(old_group);
        break;
    case P31M:
        new_group = SymmetryGroupFactory::get_instance().p31m(old_group);
        break;
    case P6:
        new_group = SymmetryGroupFactory::get_instance().p6(old_group);
        break;
    case P6M:
        new_group = SymmetryGroupFactory::get_instance().p6m(old_group);
        break;
    }

    do_action(new ActionChangeSymmetryGroup(old_group, new_group));
    set_tool(SELECT);
}

// Cancel whatever the current tool is doing.
// If the current tool is not in the middle of anything, then switch to the select tool
void ConstellationApp::handle_escape() {
    if (!current_tool->handle_escape()) {
        set_tool(SELECT);
    }
}

void ConstellationApp::handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam) {
    // Unpack parameters
    int x_pos = GET_X_LPARAM(lParam);
    int y_pos = GET_Y_LPARAM(lParam);
    int key_state = GET_KEYSTATE_WPARAM(wParam);

    // Give the canvas a chance to handle the event. If it can't, the current tool should handle it.
    if (canvas.handle_mouse_event(message, { x_pos, y_pos }, key_state))
        return;

    // translate cursor position to world space before passing to tool
    Gdiplus::PointF cursor_pos((float)x_pos, (float)y_pos);
    canvas.page_to_world_coordinates(&cursor_pos);

    Action* action = current_tool->handle_mouse_event(message, cursor_pos, key_state, canvas.get_scale());
    if (action != nullptr)
        do_action(action);
}

void ConstellationApp::handle_mouse_wheel_event(UINT message, WPARAM wParam, LPARAM lParam) {
    // Unpack parameters
    int x_pos_window = GET_X_LPARAM(lParam);
    int y_pos_window = GET_Y_LPARAM(lParam);
    int key_state = GET_KEYSTATE_WPARAM(wParam);
    int wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam);

    canvas.handle_mouse_wheel_event(message, x_pos_window, y_pos_window, key_state, wheel_delta);
}

void ConstellationApp::resize(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    canvas.resize(hWnd, wParam, lParam);
}

void ConstellationApp::draw(HWND hWnd) {
    canvas.begin_draw(hWnd, ghost, defaults);

    float scale = canvas.get_scale();
    std::vector<std::shared_ptr<Gdiplus::Matrix>> transforms = drawing.get_symmetry_group()->get_transforms();

    Gdiplus::PointF cursor_pos = get_cursor_pos(hWnd);

    // drawing
    if (ghost) {
        drawing.draw(canvas.ghost_graphics);                // Draw entire drawing at reduced opacity
        drawing.draw_original(canvas.graphics);             // Draw only original elements, on top, at full opacity
    }
    else {
        drawing.draw(canvas.graphics);
    }

    // symmetry group
    if (view_symmetry)
        drawing.get_symmetry_group()->draw(canvas.graphics, defaults, scale);

    // current tool
    current_tool->draw(canvas.graphics, cursor_pos, transforms, scale);

    canvas.finish_draw(ghost);
}

void ConstellationApp::refresh_if_necessary() {
    auto now = std::chrono::high_resolution_clock::now();
    auto frame_time_ns = now - previous_refresh_time;
    if (frame_time_ns.count() > refresh_interval_ns) {
        *Logger::get_instance() << "frame time: " << frame_time_ns.count() << std::endl;
        previous_refresh_time = now;
        canvas.redraw();
    }

}

void ConstellationApp::set_cursor() {
    current_tool->set_application_cursor();
}

void ConstellationApp::do_action(Action* action) {
    action->apply(drawing);
    current_tool->update();
    canvas.redraw();

    undo_stack.push(action);

    if (unsaved_changes < 0)
        unchanged_state_reachable = false; // This action prevents us from using redo to get to an unchanged state.
    ++unsaved_changes;

    // Since we are performing a new action, clear the redo stack.
    Action* stale_action;
    while (redo_stack.size() > 0) {
        stale_action = redo_stack.top();
        delete stale_action;
        redo_stack.pop();
    }

    *Logger::get_instance() << "unsaved_changes: " << unsaved_changes << ", unchanged_state_reachable: " << unchanged_state_reachable << std::endl;
}

void ConstellationApp::save_file() {
    std::ofstream stream(drawing_file_path);
    stream << drawing.to_json().dump();
    stream.close();

    reset_unsaved_changes();
}

void ConstellationApp::open_file(HWND hWnd) {
    std::ifstream stream(drawing_file_path);
    json drawing_json = json::parse(stream);
    drawing = Drawing(hWnd, drawing_json);
    stream.close();

    canvas.fit_drawing(drawing.get_bounding_box());
    reset_unsaved_changes();
    reset_history();

    set_tool(SELECT);
}

void ConstellationApp::reset_unsaved_changes() {
    unsaved_changes = 0;
    unchanged_state_reachable = true;
}

void ConstellationApp::reset_history() {
    while (!undo_stack.empty()) {
        delete undo_stack.top();
        undo_stack.pop();
    }
    while (!redo_stack.empty()) {
        delete redo_stack.top();
        redo_stack.pop();
    }
}

std::vector<std::shared_ptr<Element>> ConstellationApp::clone_elements(std::vector<std::shared_ptr<Element>> elements) {
    std::vector<std::shared_ptr<Element>> clones;
    for (const auto& element : elements) {
        clones.push_back(element->clone());
    }
    return clones;
}

Gdiplus::PointF ConstellationApp::get_cursor_pos(HWND hWnd) {
    POINT cursor_pos_client;
    GetCursorPos(&cursor_pos_client);                       // screen coords
    ScreenToClient(hWnd, &cursor_pos_client);               // client area coords
    Gdiplus::PointF cursor_pos_world((float)cursor_pos_client.x, (float)cursor_pos_client.y);
    canvas.page_to_world_coordinates(&cursor_pos_world);    // world coords
    return cursor_pos_world;
}
