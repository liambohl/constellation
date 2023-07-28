#include "json_converters.h"

json color_to_json(Gdiplus::Color color) {
	return {
		{"A", color.GetA()},
		{"R", color.GetR()},
		{"G", color.GetG()},
		{"B", color.GetB()}
	};
}

Gdiplus::Color color_from_json(json color_json) {
	byte A = color_json["A"];
	byte R = color_json["R"];
	byte G = color_json["G"];
	byte B = color_json["B"];
	return Gdiplus::Color(A, R, G, B);
}

json pen_to_json(Gdiplus::Pen* pen) {
	Gdiplus::Color color;
	pen->GetColor(&color);
	return {
		{"color", color_to_json(color)},
		{"width", pen->GetWidth()}
	};
}

Gdiplus::Pen* pen_from_json(json pen_json) {
	Gdiplus::Color color = color_from_json(pen_json["color"]);
	Gdiplus::REAL width = pen_json["width"];
	return new Gdiplus::Pen(color, width);
}

json brush_to_json(Gdiplus::Brush* brush) {
	Gdiplus::Color color;
	((Gdiplus::SolidBrush*)brush)->GetColor(&color);
	return {
		{"type", "SolidBrush"},
		{"color", color_to_json(color)}
	};
}

Gdiplus::Brush* brush_from_json(json brush_json) {
	std::string type = brush_json["type"];
	if (type == "SolidBrush") {
		Gdiplus::Color color = color_from_json(brush_json["color"]);
		return new Gdiplus::SolidBrush(color);
	}
}

json point_to_json(Gdiplus::PointF point) {
	return {
		{"X", point.X},
		{"Y", point.Y}
	};
}

Gdiplus::PointF point_from_json(json point_json) {
	int X = point_json["X"];
	int Y = point_json["Y"];
	return Gdiplus::PointF(X, Y);
}
