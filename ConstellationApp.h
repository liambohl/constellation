#pragma once
#include <chrono>
#include <stack>

#include "Action.h"
#include "Drawing.h"
#include "Tool.h"
#include "Path.h"

namespace Constellation {

	class ConstellationApp
	{
	private:
		Canvas canvas;
		Drawing drawing;
		Tool* current_tool = nullptr;
		std::stack<Action*> undo_stack; // bottom is oldest action performed and not undone. top is youngest action performed and not undone.
		std::stack<Action*> redo_stack; // bottom is youngest action undone. top is oldest action ondone. The undo and redo stacks are kissing at the present moment. :)

		int refresh_interval_ns;
		//void refresh_periodically(int refresh_rate);
		std::chrono::time_point<std::chrono::steady_clock> previous_refresh_time;

		void do_action(Action* action);

	public:
		ConstellationApp();

		void draw(HWND hWnd);

		// Edit menu
		void undo();
		void redo();

		// Draw menu
		void set_tool_new_path();
		void set_tool_select();

		void handle_escape();

		void handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam);

		void refresh_if_necessary();
	};

}
