#include <windowsx.h>
#include <math.h>

#include "Canvas.h"
#include "Logger.h"


const float Canvas::DEFAULT_SCALE = 5.0f;			// About 5 pixels per millimeter; should be somewhat close to actual size
const float Canvas::ZOOM_FIT_FACTOR = 0.75f;		// After fitting canvas to a drawing, zoom out by this factor
const float Canvas::GHOST_OPACITY = 0.4f;			// If "show transformed copies as ghosts" is selected, this is the opacity of those ghosts.

Canvas::Canvas() {
	transform = new Gdiplus::Matrix;

	// Color transform that multiplies the alpha channel of a color vector by GHOST_OPACITY
	Gdiplus::ColorMatrix colorMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, GHOST_OPACITY, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	ghost_attributes.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);
}

Canvas::~Canvas() {
	delete transform;
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

void Canvas::begin_draw(HWND hWnd, bool ghost, const Defaults& defaults) {
	this->hWnd = hWnd;
	hdc = BeginPaint(hWnd, &ps);
	int windowWidth = (int)(ps.rcPaint.right - ps.rcPaint.left);
	int windowHeight = (int)(ps.rcPaint.bottom - ps.rcPaint.top);
	
	// Create new screen buffer and graphics
	delete screen_buffer, graphics;
	screen_buffer = new Gdiplus::Bitmap(windowWidth, windowHeight, PixelFormat32bppARGB);
	graphics = new Gdiplus::Graphics(screen_buffer);
	graphics->SetTransform(transform);
	graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);

	if (ghost) {
		// Create new ghost screen buffer and graphics
		delete ghost_buffer, ghost_graphics;
		ghost_buffer = new Gdiplus::Bitmap(windowWidth, windowHeight, PixelFormat32bppARGB);
		ghost_graphics = new Gdiplus::Graphics(ghost_buffer);
		ghost_graphics->SetTransform(transform);
		ghost_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
	}

	graphics->Clear(defaults.background_color);
}

void Canvas::finish_draw(bool ghost) {
	if (ghost) {
		float scale = get_scale();
		Gdiplus::Matrix transform;
		graphics->GetTransform(&transform);

		// Region of the ghost_buffer to draw (all of it)
		Gdiplus::RectF source_rect = { 0.0f, 0.0f, (float)ghost_buffer->GetWidth(), (float)ghost_buffer->GetHeight() };
		// Region in world space that the drawn image should occupy (the visible region)
		Gdiplus::RectF dest_rect = { -transform.OffsetX() / scale, -transform.OffsetY() / scale, (float)ghost_buffer->GetWidth() / scale, (float)ghost_buffer->GetHeight() / scale };
		
		// Draw ghost image at reduced opacity
		// To pass ImageAttributes in a call to Graphics::DrawImage, we have to use this ugly overload.
		graphics->DrawImage(
			ghost_buffer,
			dest_rect,
			source_rect,
			Gdiplus::UnitPixel,
			&ghost_attributes
		);
	}
	
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

	pan(client_width / 2.0f, client_height / 2.0f);
	zoom(DEFAULT_SCALE, client_width / 2.0f, client_height / 2.0f);
}

void Canvas::fit_drawing(std::optional<Gdiplus::RectF> bounding_box) {
	if (!bounding_box) {
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

float Canvas::get_scale() {
	// Get the matrix that represents the current transformation from world coordinates to page coordinates.
	Gdiplus::Matrix transform;
	graphics->GetTransform(&transform);
	float elements[6];
	transform.GetElements(elements);

	// Assume no shear or rotation in the world-to-page transform
	float x_scale = elements[0];
	float y_scale = elements[3];
	return sqrtf(x_scale * y_scale);
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
}
