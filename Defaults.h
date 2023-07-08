#pragma once
#include "framework.h"

namespace Constellation {

	class Defaults
	{
	public:
		//Gdiplus::Color tool_stroke_color = Gdiplus::Color(255, 255, 0, 0);
		//Gdiplus::REAL tool_stroke_width = 1.0f;
		//Gdiplus::Color stroke_color = Gdiplus::Color(255, 0, 0, 0);
		//Gdiplus::REAL stroke_width = 3.0f;
		//Gdiplus::Color fill_color = Gdiplus::Color(255, 255, 255, 255);
		Gdiplus::Pen* pen = nullptr;			// elements in the drawing
		Gdiplus::Pen* wip_pen = nullptr;		// works in progress: elements currently being created by a tool
		Gdiplus::Pen* tool_pen = nullptr;		// the cutting edge
		Gdiplus::Brush* brush = nullptr;		// elements in the drawing
		Gdiplus::Color background;				// "transparent" color shown where there are no elements

		//Gdiplus::Pen* clone_tool_pen() { return tool_pen->Clone(); }
		//Gdiplus::Pen* clone_pen() { return pen->Clone(); }
		//Gdiplus::Brush* clone_brush() { return brush->Clone(); }

		Defaults();
	};

}
