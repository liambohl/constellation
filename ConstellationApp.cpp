#include <thread>
#include <windowsx.h>

#include "ToolNewPath.h"
#include "ToolSelect.h"
#include "ConstellationApp.h"
#include "Logger.h"

namespace Constellation {

    ConstellationApp::ConstellationApp() :
        canvas(1.0, 0.0, 0.0)
    {
        current_tool = new ToolSelect(drawing);

        // Get screen refresh rate and calculate refresh interval
        DEVMODEA* dm = new DEVMODEA();
        dm->dmSize = sizeof(DEVMODEA);
        EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, dm);
        int refresh_rate = dm->dmDisplayFrequency;
        if (refresh_rate < 59)
            refresh_rate = 60;
        refresh_interval_ns = 1000000000 / refresh_rate;
        *Logger::get_instance() << "refresh interval: " << refresh_interval_ns << std::endl;

        //previous_refresh_time = std::chrono::high_resolution_clock::now();

        //*Logger::get_instance() << "refresh rate: " << refresh_rate << std::endl;
        //std::thread refresh_thread(&ConstellationApp::refresh_periodically, this, refresh_rate);
        //refresh_thread.detach();
	}

    //void ConstellationApp::refresh_periodically(int refresh_rate) {
    //    int refresh_interval_ns = 1000000000 / refresh_rate;
    //    *Logger::get_instance() << "refresh interval: " << refresh_interval_ns << std::endl;
    //    while (true) {
    //        auto now = std::chrono::high_resolution_clock::now();
    //        auto frame_time = now - previous_refresh_time;
    //        *Logger::get_instance() << "frame time: " << frame_time.count() << std::endl;
    //        previous_refresh_time = now;
    //        canvas.redraw();
    //        std::this_thread::sleep_for(std::chrono::nanoseconds(refresh_interval_ns));
    //    }
    //}

    void ConstellationApp::draw(HWND hWnd) {
        canvas.begin_draw(hWnd);
        drawing.draw(canvas);
        current_tool->draw(canvas);
        canvas.finish_draw();
    }

    void ConstellationApp::new_drawing() {
        drawing_file_path = nullptr;
        drawing_folder = nullptr;
        // TODO: new file
        *Logger::get_instance() << "New file" << std::endl;
    }

    void ConstellationApp::open() {
        if (open_cst_file(&drawing_file_path, drawing_folder)) {
            // TODO: open file
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
            // TODO: save file
            *Logger::get_instance() << "Saved file \"" << drawing_file_path << "\"" << std::endl;
        }
    }

    void ConstellationApp::save_as() {
        if (save_as_cst_file(&drawing_file_path, drawing_folder)) {
            // TODO: save file
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
        }
    }

    void ConstellationApp::redo() {
        if (redo_stack.size() > 0) {
            Action* next = redo_stack.top();
            next->apply(drawing);
            redo_stack.pop();
            undo_stack.push(next);
        }
    }

    void ConstellationApp::set_tool_new_path() {
        delete current_tool;
        current_tool = new ToolNewPath(drawing);
    }

    void ConstellationApp::set_tool_select() {
        delete current_tool;
        current_tool = new ToolSelect(drawing);
    }

    // Cancel whatever the current tool is doing.
    // If the current tool is not in the middle of anything, then switch to the select tool
    void ConstellationApp::handle_escape() {
        if (!current_tool->handle_escape()) {
            set_tool_select();
        }
    }

    void ConstellationApp::do_action(Action* action) {
        action->apply(drawing);
        canvas.redraw();

        undo_stack.push(action);

        // Since we are performing a new action, clear the redo stack.
        Action* stale_action;
        while (redo_stack.size() > 0) {
            stale_action = redo_stack.top();
            delete stale_action;
            redo_stack.pop();
        }
    }

    void ConstellationApp::handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam) {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        Action* action = current_tool->handle_mouse_event(message, wParam, lParam);
        if (action != nullptr)
            do_action(action);
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

}
