#pragma once
#include "framework.h"

// This class contains information about how
// the current drawing should be drawn to the current window.
class Canvas
{
private:
	void pan(float delta_x, float delta_y);
	void zoom(float scale_factor, float x_pos, float y_pos);

	PAINTSTRUCT ps;
	HWND hWnd = nullptr;						// handle to the window
	Gdiplus::Image* screen_buffer = nullptr;	// Image that we draw to before drawing to the screen
	
	Gdiplus::Matrix* transform;					// transformation from world space to page space

	const float DEFAULT_SCALE = 5.0f;			// About 5 pixels per millimeter; should be somewhat close to actual size
	const float ZOOM_FIT_FACTOR = 0.75f;		// After fitting canvas to a drawing, zoom out by this factor

	// Cursor position, in pixels, measured from top-left of client area, last time a mouse event was handled.
	// Used for panning the view with middle mouse button drag.
	int prev_x_pos = 0;
	int prev_y_pos = 0;

	// Window size, in pixels
	int prev_width = 0;
	int prev_height = 0;

public:
	Canvas();
	~Canvas();

	// If this canvas can handle the event, do so and return true. Else, return false.
	bool handle_mouse_event(UINT message, int x_pos, int y_pos, int key_state);
	bool handle_mouse_wheel_event(UINT message, int x_pos_window, int y_pos_window, int key_state, int wheel_delta);

	void resize(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void begin_draw(HWND hWnd);
	void finish_draw();
	void redraw();

	void reset_transform();
	void reset_transform(int client_width, int client_height);
	void fit_drawing(Gdiplus::RectF* bounding_box);

	void page_to_world_coordinates(Gdiplus::PointF* point_page);

	LONG windowWidth = 0;						// in window pixels
	LONG windowHeight = 0;						// in window pixels

	HDC hdc = NULL;								// Handle to device context; this is the window we paint to
	Gdiplus::Graphics* graphics = nullptr;		// GDI+ graphics object we use to draw
};
