#include <windowsx.h>
#include <math.h>

#include "Canvas.h"
#include "Logger.h"


Canvas::Canvas() {
	transform = new Gdiplus::Matrix;
}

Canvas::~Canvas() {
	delete transform;
}

// Pan the canvas by the given delta, measured in pixels.
void Canvas::pan(float delta_x, float delta_y) {
	transform->Translate(delta_x, delta_y, Gdiplus::MatrixOrderAppend);
}

// Zoom in or out by a given factor, around the cursor.
// Zoom in if scale_factor > 1; out if scale_factor < 1.
// (x_pos, y_pos) is the cursor position in page coordinates.
void Canvas::zoom(float scale_factor, float x_pos, float y_pos) {
	transform->Translate(-x_pos, -y_pos, Gdiplus::MatrixOrderAppend);
	transform->Scale(scale_factor, scale_factor, Gdiplus::MatrixOrderAppend);
	transform->Translate(x_pos, y_pos, Gdiplus::MatrixOrderAppend);
	scale *= scale_factor;
}

bool Canvas::handle_mouse_event(UINT message, int x_pos, int y_pos, int key_state) {
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
		pan((float)(x_pos - prev_x_pos), (float)(y_pos - prev_y_pos));
		prev_x_pos = x_pos;
		prev_y_pos = y_pos;
		return true;
	}

	return false;
}

bool Canvas::handle_mouse_wheel_event(UINT message, int x_pos_window, int y_pos_window, int key_state, int wheel_delta) {
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
		pan(0, (float)wheel_delta);
		return true;
	}

	// Pan horizontally with horizontal scroll
	if (message == WM_MOUSEHWHEEL && !shift_pressed && !control_pressed) {
		pan(-(float)wheel_delta, 0);
		return true;
	}

	// Pan horizontally with shift-scroll
	if (message == WM_MOUSEWHEEL && shift_pressed && !control_pressed) {
		pan((float)wheel_delta, 0);
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

void Canvas::resize(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if (wParam != SIZE_MINIMIZED) {
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		*Logger::get_instance() << "rezising. width = " << width << ", height = " << height << std::endl;

		// Keep drawing centered in the client area
		pan((width - prev_width) / 2.0f, (height - prev_height) / 2.0f);

		// Zoom to fill the same amount of the client area
		if (prev_width != 0 && prev_height != 0) {
			float scale_x = (float)width / prev_width;
			float scale_y = (float)height / prev_height;
			float scale = powf(scale_x * scale_y, 0.5);	// geometric mean
			*Logger::get_instance() << "rezising. scale_x = " << scale_x << ", scale_y = " << scale_y << ", scale = " << scale << std::endl;
			zoom(scale, width / 2.0f, height / 2.0f);
		}
		else if (width != 0 && height != 0) {	// This is probably a new window which has just been given a size
			reset_transform(width, height);
		}

		prev_width = width;
		prev_height = height;
	}
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
	graphics->SetTransform(transform);

	try {
		graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
	}
	catch (std::exception e) {
		*Logger::get_instance() << e.what();
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

void Canvas::reset_transform() {
	// Get client area size
	RECT rect;
	GetClientRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	reset_transform(width, height);
}

void Canvas::reset_transform(int client_width, int client_height) {
	delete transform;
	transform = new Gdiplus::Matrix();
	scale = 1.0;

	pan(client_width / 2.0f, client_height / 2.0f);
	zoom(DEFAULT_SCALE, client_width / 2.0f, client_height / 2.0f);
}

void Canvas::fit_drawing(Gdiplus::RectF* bounding_box) {
	if (bounding_box == nullptr) {
		reset_transform();
		return;
	}

	delete transform;
	transform = new Gdiplus::Matrix();

	// Get client area size
	RECT rect;
	GetClientRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	float scale = ZOOM_FIT_FACTOR * min(width / bounding_box->Width, height / bounding_box->Height);

	// Centers of drawing and window
	float drawing_center_x = bounding_box->X + bounding_box->Width / 2.0f;
	float drawing_center_y = bounding_box->Y + bounding_box->Height / 2.0f;
	float window_center_x = width / 2.0f;
	float window_center_y = height / 2.0f;

	pan(window_center_x - drawing_center_x, window_center_y - drawing_center_y);
	zoom(scale, window_center_x, window_center_y);
}

void Canvas::page_to_world_coordinates(Gdiplus::PointF* point_page) {
	transform->Invert();
	transform->TransformPoints(point_page);
	transform->Invert();
}
