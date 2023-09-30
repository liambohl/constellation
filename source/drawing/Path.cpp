#include "Path.h"

#include <string>

#include "json_converters.h"
#include "core/Logger.h"


Path::Path(json path_json):
	pen(pen_from_json(path_json["pen"]))
{
	std::vector<Gdiplus::PointF> control_points;
	path = path_from_json(path_json["path"]);
}

Path::~Path() {
	delete path, pen;
}

std::optional<Gdiplus::RectF> Path::get_bounding_box() {
	if (path->GetPointCount() == 0)
		return {};
	Gdiplus::GraphicsPath* flat_path = path->Clone();
	flat_path->Flatten();
	Gdiplus::RectF bounding_box;
	flat_path->GetBounds(&bounding_box, nullptr, pen);
	return bounding_box;
}

bool Path::try_select(const Gdiplus::PointF& cursor_pos) {
	return path->IsOutlineVisible(cursor_pos, pen);
}

json Path::to_json() {
	return {
		{"type", "Path"},
		{"pen", pen_to_json(pen)},
		{"path", path_to_json(path)}
	};
}

void Path::add_beziers(std::vector<Gdiplus::PointF> control_points) {
	path->AddBeziers(control_points.data(), (int)control_points.size());
}

void Path::draw_one(Gdiplus::Graphics* graphics) {
	graphics->DrawPath(pen, path);
}
