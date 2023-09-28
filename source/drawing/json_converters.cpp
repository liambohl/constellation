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
	auto pen = new Gdiplus::Pen(color, width);
	pen->SetStartCap(Gdiplus::LineCapRound);
	pen->SetEndCap(Gdiplus::LineCapRound);
	pen->SetLineJoin(Gdiplus::LineJoinRound);
	return pen;
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
	else {
		return nullptr;
	}
}

json point_to_json(Gdiplus::PointF point) {
	return {
		{"X", point.X},
		{"Y", point.Y}
	};
}

Gdiplus::PointF point_from_json(json point_json) {
	float X = point_json["X"];
	float Y = point_json["Y"];
	return Gdiplus::PointF(X, Y);
}

json path_to_json(Gdiplus::GraphicsPath* path) {
	Gdiplus::PathData path_data;
	path->GetPathData(&path_data);
	std::vector<json> points_json;
	std::vector<json> types_json;
	for (int i = 0; i < path_data.Count; ++i) {
		points_json.push_back(point_to_json(path_data.Points[i]));
		types_json.push_back(path_data.Types[i]);
	}

	return {
		{"count", path_data.Count},
		{"points", points_json},
		{"point_types", types_json},
		{"fill_mode", path->GetFillMode() == Gdiplus::FillModeAlternate ? "alternate" : "winding"}
	};
}

Gdiplus::GraphicsPath* path_from_json(json path_json) {
	int count = path_json["count"];
	Gdiplus::PointF* points = new Gdiplus::PointF[count];
	byte* types = new byte[count];
	std::vector<json> points_json = path_json["points"];
	std::vector<json> types_json = path_json["point_types"];
	for (int i = 0; i < count; ++i) {
		points[i] = point_from_json(points_json[i]);
		types[i] = types_json[i];
	}
	Gdiplus::FillMode fill_mode = path_json["fill_mode"] == "alternate" ? Gdiplus::FillModeAlternate : Gdiplus::FillModeWinding;

	return new Gdiplus::GraphicsPath(points, types, count, fill_mode);
}
