#include <fstream>
#include <thread>
#include <windowsx.h>

#include "ToolNewPath.h"
#include "ToolSelect.h"
#include "ConstellationApp.h"
#include "Logger.h"

ConstellationApp::ConstellationApp()
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
    *Logger::get_instance() << "refresh interval: " << refresh_interval_ns << std::endl;
}

void ConstellationApp::resize(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    canvas.resize(hWnd, wParam, lParam);
}

void ConstellationApp::draw(HWND hWnd) {
    canvas.begin_draw(hWnd);
    drawing.draw(canvas);
    current_tool->draw(canvas);
    canvas.finish_draw();
}

void ConstellationApp::new_drawing() {
    drawing_file_path = nullptr;
    drawing_folder = nullptr;
    drawing = Drawing();
    reset_unsaved_changes();
    reset_history();
    *Logger::get_instance() << "New file" << std::endl;
}

void ConstellationApp::open() {
    if (open_cst_file(&drawing_file_path, drawing_folder)) {
        open_file();
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

        --unsaved_changes;
    }
}

void ConstellationApp::redo() {
    if (redo_stack.size() > 0) {
        Action* next = redo_stack.top();
        next->apply(drawing);
        redo_stack.pop();
        undo_stack.push(next);

        ++unsaved_changes;
    }
}

void ConstellationApp::set_tool(enum tool tool_type) {
    delete current_tool;
    switch (tool_type) {
    case NEW_PATH:
        current_tool = new ToolNewPath(defaults);
        break;
    case SELECT:
        current_tool = new ToolSelect(defaults);
        break;
    }
}

// Cancel whatever the current tool is doing.
// If the current tool is not in the middle of anything, then switch to the select tool
void ConstellationApp::handle_escape() {
    if (!current_tool->handle_escape()) {
        set_tool(SELECT);
    }
}

void ConstellationApp::do_action(Action* action) {
    action->apply(drawing);
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

void ConstellationApp::open_file() {
    std::ifstream stream(drawing_file_path);
    json drawing_json = json::parse(stream);
    drawing = Drawing(drawing_json);
    stream.close();

    // TODO: reset canvas
    reset_unsaved_changes();
    reset_history();
}

void ConstellationApp::reset_unsaved_changes() {
    unsaved_changes = 0;
    unchanged_state_reachable = true;
}

// Clear undo and redo stacks and free the action pointers therein
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

void ConstellationApp::handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam) {
    // Give the canvas a chance to handle the event. If it can't, the current tool should handle it.
    if (canvas.handle_mouse_event(message, wParam, lParam))
        return;

    Action* action = current_tool->handle_mouse_event(message, wParam, lParam);
    if (action != nullptr)
        do_action(action);
}

void ConstellationApp::handle_mouse_wheel_event(UINT message, WPARAM wParam, LPARAM lParam) {
    canvas.handle_mouse_wheel_event(message, wParam, lParam);
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
