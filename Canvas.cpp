#include <windowsx.h>
#include <math.h>

#include "Canvas.h"
#include "Logger.h"


// Pan the canvas by the given delta, measured in pixels.
void Canvas::pan(int delta_x, int delta_y) {
	transform.Translate((float)delta_x, (float)delta_y, Gdiplus::MatrixOrderAppend);
}

// Zoom in or out by a given factor, around the cursor.
// Zoom in if scale_factor > 1; out if scale_factor < 1.
// (x_pos, y_pos) is the cursor position in page coordinates.
void Canvas::zoom(float scale_factor, float x_pos, float y_pos) {
	transform.Translate(-x_pos, -y_pos, Gdiplus::MatrixOrderAppend);
	transform.Scale(scale_factor, scale_factor, Gdiplus::MatrixOrderAppend);
	transform.Translate(x_pos, y_pos, Gdiplus::MatrixOrderAppend);
}

bool Canvas::handle_mouse_event(UINT message, WPARAM wParam, LPARAM lParam) {
	// Unpack parameters
	int x_pos = GET_X_LPARAM(lParam);
	int y_pos = GET_Y_LPARAM(lParam);
	int key_state = GET_KEYSTATE_WPARAM(wParam);

	// Unpack key state
	bool middle_button_down = key_state & MK_MBUTTON;

	// Start pan with middle mouse button
	if (message == WM_MBUTTONDOWN) {
		prev_x_pos = x_pos;
		prev_y_pos = y_pos;
		return true;
	}

	// Pan with middle mouse button drag
	if (message == WM_MOUSEMOVE && middle_button_down) {
		pan(x_pos - prev_x_pos, y_pos - prev_y_pos);
		prev_x_pos = x_pos;
		prev_y_pos = y_pos;
		return true;
	}

	return false;
}

bool Canvas::handle_mouse_wheel_event(UINT message, WPARAM wParam, LPARAM lParam) {
	// Unpack parameters
	int x_pos_window = GET_X_LPARAM(lParam);
	int y_pos_window = GET_Y_LPARAM(lParam);
	int key_state = GET_KEYSTATE_WPARAM(wParam);
	int wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam);

	// Unpack key state
	bool shift_pressed = key_state & MK_SHIFT;
	bool control_pressed = key_state & MK_CONTROL;

	// Convert window coordinates to page coordinates
	// (For mouse wheel events, the mouse position is bizarrely given in window coordinates)
	POINT cursor_pos{ x_pos_window, y_pos_window };
	ScreenToClient(hWnd, &cursor_pos);
	float x_pos = (float)cursor_pos.x;
	float y_pos = (float)cursor_pos.y;

	// Pan vertically with scroll
	if (message == WM_MOUSEWHEEL && !shift_pressed && !control_pressed) {
		pan(0, wheel_delta);
		return true;
	}

	// Pan horizontally with horizontal scroll
	if (message == WM_MOUSEHWHEEL && !shift_pressed && !control_pressed) {
		pan(-wheel_delta, 0);
		return true;
	}

	// Pan horizontally with shift-scroll
	if (message == WM_MOUSEWHEEL && shift_pressed && !control_pressed) {
		pan(wheel_delta, 0);
		return true;
	}

	// Zoom with ctrl-scroll
	if (message == WM_MOUSEWHEEL && !shift_pressed && control_pressed) {
		float scale_factor = powf(1.25f, (float)wheel_delta / WHEEL_DELTA);	// One "wheel delta" of scrolling should scale by a factor of 5/4
		zoom(scale_factor, x_pos, y_pos);
		return true;
	}
	
	return false;
}

void Canvas::begin_draw(HWND hWnd) {
	this->hWnd = hWnd;
	hdc = BeginPaint(hWnd, &ps);
	windowWidth = ps.rcPaint.right - ps.rcPaint.left;
	windowHeight = ps.rcPaint.bottom - ps.rcPaint.top;
	
	delete screen_buffer;
	screen_buffer = new Gdiplus::Bitmap(windowWidth, windowHeight, PixelFormat32bppARGB);
	delete graphics;
	graphics = new Gdiplus::Graphics(screen_buffer);
	graphics->SetTransform(&transform);

	try {
		graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
	}
	catch (std::exception e) {
		Logger::get_instance()->log(e.what());
	}
}

void Canvas::finish_draw() {
	Gdiplus::Graphics true_graphics(hdc);
	true_graphics.DrawImage(screen_buffer, Gdiplus::Point(0, 0));
	EndPaint(hWnd, &ps);
}

void Canvas::redraw() {
	InvalidateRect(hWnd, NULL, FALSE);
}
