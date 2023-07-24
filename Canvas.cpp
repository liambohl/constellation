#include "Canvas.h"
#include "Logger.h"

Canvas::Canvas(double zoom, double offsetX, double offsetY) :
	zoom(zoom),
	offsetX(offsetX),
	offsetY(offsetY)
{

}
	
Canvas::~Canvas()
{
	//delete graphics;
	EndPaint(hWnd, &ps);
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
	try {
		graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
	}
	catch (std::exception e) {
		Logger::get_instance()->log(e.what());
	}
	int x = 4;
}

void Canvas::finish_draw() {
	Gdiplus::Graphics true_graphics(hdc);
	true_graphics.DrawImage(screen_buffer, Gdiplus::Point(0, 0));
}

void Canvas::redraw() {
	InvalidateRect(hWnd, NULL, FALSE);
}
