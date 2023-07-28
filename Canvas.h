#pragma once
#include "framework.h"

// This class contains information about how
// the current drawing should be drawn to the current window.
class Canvas
{
private:
	void pan(int delta_x, int delta_y);
	void zoom(float scale_factor, float x_pos, float y_pos);

	PAINTSTRUCT ps;
	HWND hWnd = nullptr;						// handle to the window
	Gdiplus::Image* screen_buffer = nullptr;	// Image that we draw to before drawing to the screen

	// Cursor position, in pixels, measured from top-left of client area, last time a mouse event was handled.
	// Used for panning the view with middle mouse button drag.
	int prev_x_pos = 0;
	int prev_y_pos = 0;

	// Window size, in pixels
	int prev_width = 0;
	int prev_height = 0;

public:
	// If this canvas can handle the event, do so and return true. Else, return false.
	bool handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam);
	bool handle_mouse_wheel_event(UINT message, WPARAM wParam, LPARAM lParam);

	void resize(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void begin_draw(HWND hWnd);
	void finish_draw();
	void redraw();

	Gdiplus::Matrix transform;					// transformation from world space to page space

	LONG windowWidth = 0;						// in window pixels
	LONG windowHeight = 0;						// in window pixels

	HDC hdc = NULL;								// Handle to device context; this is the window we paint to
	Gdiplus::Graphics* graphics = nullptr;		// GDI+ graphics object we use to draw
};
