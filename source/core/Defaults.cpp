#include "Defaults.h"

Defaults::Defaults() {
	pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0), 1.0f);
	wip_pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 255, 255), 0.0f);
	tool_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 0, 0), 0.0f);
	brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 255));
}
