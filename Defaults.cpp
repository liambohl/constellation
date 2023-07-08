#include "Defaults.h"

namespace Constellation {

	Defaults::Defaults() {
		pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0), 3.0f);
		wip_pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 255, 255), 1.0f);
		tool_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 0, 0), 1.0f);
		brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 255));
		background = Gdiplus::Color(255, 80, 80, 80);
	}

}