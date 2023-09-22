#pragma once

#include "framework.h"


// This class contains information about how
// the current drawing should be drawn to the current window.
class Canvas
{
public:
	Canvas();
	~Canvas();

	// If this canvas can handle the event, do so and return true. Else, return false.
	bool handle_mouse_event(UINT message, int x_pos, int y_pos, int key_state);
	bool handle_mouse_wheel_event(UINT message, int x_pos_window, int y_pos_window, int key_state, int wheel_delta);

	// Resize application window
	void resize(HWND hWnd, WPARAM wParam, LPARAM lParam);

	// Call this before drawing to the canvas.
	void begin_draw(HWND hWnd);
	// Call this when finished drawing to the canvas.
	void finish_draw();

	// Force the client area to be redrawn
	void redraw();

	// Zoom to the default scale and center the drawing in the client area
	void reset_transform();
	void reset_transform(int client_width, int client_height);

	// Pan and zoom to fit the given bounding box (in world space)
	void fit_drawing(Gdiplus::RectF* bounding_box);

	// Convert a point from client coordinates to world space
	void page_to_world_coordinates(Gdiplus::PointF* point_page);

	// ratio of window pixels to world pixels (linear; higher is more zoomed in)
	float get_scale();

	Gdiplus::Graphics* graphics = nullptr;		// GDI+ graphics object we use to draw

private:
	// zoom scale for new drawings
	static const float DEFAULT_SCALE;
	// When zooming to fit a drawing, the drawing should fill at most this proportion
	// of the client area, either vertically or horizontally.
	static const float ZOOM_FIT_FACTOR;

	// Pan the canvas by the given delta, measured in pixels.
	void pan(float delta_x, float delta_y);
	// Zoom in or out by a given factor, around the cursor.
	// Zoom in if scale_factor > 1; out if scale_factor < 1.
	// (x_pos, y_pos) is the cursor position in page coordinates.
	void zoom(float scale_factor, float x_pos, float y_pos);

	HDC hdc = nullptr;							// Handle to device context; this is the window we paint to
	PAINTSTRUCT ps = {};
	HWND hWnd = nullptr;						// handle to the window
	Gdiplus::Image* screen_buffer = nullptr;	// Image that we draw to before drawing to the screen

	Gdiplus::Matrix* transform;					// transformation from world space to page space

	// Cursor position, in pixels, measured from top-left of client area, last time a mouse event was handled.
	// Used for panning the view with middle mouse button drag.
	int prev_x_pos = 0;
	int prev_y_pos = 0;

	// Window size, in pixels
	int prev_width = 0;
	int prev_height = 0;
};
