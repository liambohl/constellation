#pragma once

#include <optional>

#include "Defaults.h"
#include "framework.h"


// This class contains information about how
// the current drawing should be drawn to the current window.
class Canvas
{
public:
    Canvas();
    ~Canvas();

    // If this canvas can handle the event, do so and return true. Else, return false.
    bool handle_mouse_event(UINT message, Gdiplus::Point cursor_pos, int key_state);
    bool handle_mouse_wheel_event(UINT message, int x_pos_window, int y_pos_window, int key_state, int wheel_delta);

    // Resize application window
    void resize(HWND hWnd, WPARAM wParam, LPARAM lParam);

    // Call this before drawing to the canvas.
    void begin_draw(HWND hWnd, bool ghost, const Defaults& defaults);
    // Call this when finished drawing to the canvas.
    void finish_draw(bool ghost);

    // Force the client area to be redrawn
    void redraw();

    // Zoom to the default scale and center the drawing in the client area
    void reset_transform();
    void reset_transform(int client_width, int client_height);

    // Pan and zoom to fit the given bounding box (in world space)
    void fit_drawing(std::optional<Gdiplus::RectF> bounding_box);

    // Convert a point from client coordinates to world space
    void page_to_world_coordinates(Gdiplus::PointF* point_page);

    // ratio of window pixels to world pixels (linear; higher is more zoomed in)
    float get_scale();

    // GDI+ graphics object we use to draw original elements, tools, guides, etc.
    Gdiplus::Graphics* graphics = nullptr;
    // GDI+ graphics object we use to draw transformed copies of elements as "ghosts" with transparency
    Gdiplus::Graphics* ghost_graphics = nullptr;

private:
    // zoom scale for new drawings
    static const float DEFAULT_SCALE;
    // When zooming to fit a drawing, the drawing should fill at most this proportion
    // of the client area, either vertically or horizontally.
    static const float ZOOM_FIT_FACTOR;
    // If "show transformed copies as ghosts" is selected, this is the opacity of those ghosts.
    static const float GHOST_OPACITY;

    // Pan the canvas by the given delta, measured in pixels.
    void pan(Gdiplus::Point delta);
    // Zoom in or out by a given factor, around the cursor.
    // Zoom in if scale_factor > 1; out if scale_factor < 1.
    // cursor_pos is the cursor position in page coordinates.
    void zoom(float scale_factor, Gdiplus::Point cursor_pos);

    HDC hdc = nullptr;                          // Handle to device context; this is the window we paint to
    PAINTSTRUCT ps = {};
    HWND hWnd = nullptr;                        // handle to the window

    Gdiplus::Image* buffer = nullptr;           // The drawing and tools are drawn to this image, which in turn is drawn to page_buffer
    Gdiplus::Image* ghost_buffer = nullptr;     // Ghosts are drawn to this image, which in turn is drawn to page_buffer at reduced opacity
    Gdiplus::Image* page_buffer = nullptr;      // Double buffer that is drawn to the screen

    Gdiplus::Graphics* page_graphics = nullptr; // This is how we draw to page_buffer

    Gdiplus::Matrix* transform;                 // transformation from world space to page space

    // Cursor position, in pixels, measured from top-left of client area, last time a mouse event was handled.
    // Used for panning the view with middle mouse button drag.
    Gdiplus::Point prev_cursor_pos;

    // Window size, in pixels
    int prev_width = 0;
    int prev_height = 0;

    // Contains a color transform to draw this image with partial opacity
    Gdiplus::ImageAttributes ghost_attributes;
};
