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
	delete pen;
}

void Path::get_bounding_box(Gdiplus::RectF** bounding_box) {
	Gdiplus::RectF path_bounds;
	Gdiplus::GraphicsPath* flat_path = path->Clone();
	flat_path->Flatten();
	flat_path->GetBounds(&path_bounds, nullptr, pen);

	if (*bounding_box == nullptr)
		*bounding_box = path_bounds.Clone();

	float left = min((**bounding_box).GetLeft(), path_bounds.GetLeft());
	float top = min((**bounding_box).GetTop(), path_bounds.GetTop());
	float right = max((**bounding_box).GetRight(), path_bounds.GetRight());
	float bottom = max((**bounding_box).GetBottom(), path_bounds.GetBottom());

	(**bounding_box).X = left;
	(**bounding_box).Y = top;
	(**bounding_box).Width =  right - left;
	(**bounding_box).Height = bottom - top;
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
