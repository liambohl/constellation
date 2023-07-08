#pragma once
#include "framework.h"

namespace Constellation {

	// This class contains information about how
	// the current drawing should be drawn to the current window.
	class Canvas
	{
	private:
		PAINTSTRUCT ps;
		HWND hWnd = nullptr;             // handle to the window
		Gdiplus::Image* screen_buffer;	 // Image that we draw to before drawing to the screen

	public:
		double zoom = 1.0;               // Drawing pixels per window pixel

		// These values represent the point on the drawing where the window is "focused"
		double offsetX = 0.0;            // X-position of center of window on canvas
		double offsetY = 0.0;            // Y-position of center of window on canvas

		LONG windowWidth;                // in window pixels
		LONG windowHeight;               // in window pixels

		HDC hdc = NULL;                  // Handle to device context; this is the window we paint to
		Gdiplus::Graphics* graphics = nullptr;    // GDI+ graphics object we use to draw

		Canvas(double zoom, double offsetX, double offsetY);
		~Canvas();
		
		void begin_draw(HWND hWnd);
		void finish_draw();
		void redraw();
	};

}