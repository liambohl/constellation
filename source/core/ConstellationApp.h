#pragma once
#include <chrono>
#include <stack>

#include <shobjidl.h>	// For IShellItem

#include "Canvas.h"
#include "Defaults.h"
#include "enums.h"
#include "file.h"
#include "actions/Action.h"
#include "drawing/Drawing.h"
#include "tools/Tool.h"

class ConstellationApp
{
private:
	// Any action which changes the drawing must do so through this function.
	// Undo, redo, file > open, and file > new are not actions and are exempt.
	void do_action(Action* action);

	void save_file();
	void open_file();

	void reset_unsaved_changes();
	void reset_history();

	Canvas canvas;
	Defaults defaults;
	Drawing drawing;
	Tool* current_tool = nullptr;

	// Name and location of currrently open file. If nullptr, we are editing a new, never-saved drawing.
	PWSTR drawing_file_path = nullptr;		// full path
	IShellItem* drawing_folder = nullptr;

	std::stack<Action*> undo_stack; // bottom is oldest action performed and not undone. top is youngest action performed and not undone.
	std::stack<Action*> redo_stack; // bottom is youngest action undone. top is oldest action ondone. The undo and redo stacks are kissing at the present moment. :)

	// If this number is positive, then that many undo's would take us back to an unchanged state.
	// If this number is negative, then that many redo's, without any new changes, would take us back to an unchanged state.
	int unsaved_changes = 0;

	// This becomes false if we make new changes while unsaved_changes < 0; that is when we save, undo some changes, then make some new changes.
	// If false, no combination of undo and redo will bring us back to an unchanged state, and unsaved_changes is meaningless.
	bool unchanged_state_reachable = true;

	int refresh_interval_ns;
	//void refresh_periodically(int refresh_rate);
	std::chrono::time_point<std::chrono::steady_clock> previous_refresh_time;

public:
	ConstellationApp();

	void resize(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void draw(HWND hWnd);

	// File menu
	void new_drawing();
	void open();
	void save();
	void save_as();

	// Edit menu
	void undo();
	void redo();

	// Draw menu
	void set_tool(enum tool tool_type);

	// Symmetry menu
	void set_symmetry_group(enum symmetry_group symmetry_group);

	void handle_escape();

	void handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam);
	void handle_mouse_wheel_event(UINT message, WPARAM wParam, LPARAM lParam);
	void refresh_if_necessary();

	// Does the current drawing have any unsaved changes?
	bool has_unsaved_changes() { return unsaved_changes != 0 || !unchanged_state_reachable; }
};
