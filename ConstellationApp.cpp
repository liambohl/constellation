#include <thread>
#include <windowsx.h>
#include "ConstellationApp.h"
#include "Logger.h"

namespace Constellation {

    ConstellationApp::ConstellationApp() :
        canvas(1.0, 0.0, 0.0)
    {
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
        currentTool->draw(canvas);
        canvas.finish_draw();
    }

    void ConstellationApp::handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam) {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        currentTool->handleMouseEvent(message, wParam, lParam);
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
