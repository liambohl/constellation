#include "Defaults.h"
#include "json_converters.h"

Defaults::Defaults() {
	pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0), 1.0f);
	wip_pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 255, 255), 0.0f);
	tool_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 0, 0), 0.0f);
	brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 255));
}

Defaults::Defaults(json defaults_json) {
	pen = pen_from_json(defaults_json["pen"]);
	wip_pen = pen_from_json(defaults_json["wip_pen"]);
	tool_pen = pen_from_json(defaults_json["tool_pen"]);
	brush = brush_from_json(defaults_json["brush"]);
}

json Defaults::to_json() {
	return {
		{"pen", pen_to_json(pen)},
		{"wip_pen", pen_to_json(wip_pen)},
		{"tool_pen", pen_to_json(tool_pen)},
		{"brush", brush_to_json(brush)},
	};
}
