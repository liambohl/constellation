#pragma once
#include <chrono>

#include "Drawing.h"
#include "Tool.h"
#include "Path.h"
#include "BezierTool.h"

namespace Constellation {

	class ConstellationApp
	{
	private:
		Canvas canvas;
		Drawing drawing;
		int refresh_interval_ns;

		Tool* currentTool = new BezierTool(drawing);

		void refresh_periodically(int refresh_rate);
		std::chrono::time_point<std::chrono::steady_clock> previous_refresh_time;

	public:
		ConstellationApp();

		void draw(HWND hWnd);

		void handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam);

		void refresh_if_necessary();
	};

}
