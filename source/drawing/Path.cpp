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
	delete path;
	delete pen;
}

std::optional<Gdiplus::RectF> Path::get_bounding_box() {
	if (path->GetPointCount() == 0)
		return {};
	Gdiplus::GraphicsPath* flat_path = path->Clone();
	flat_path->Flatten();
	Gdiplus::RectF bounding_box;
	flat_path->GetBounds(&bounding_box, nullptr, pen);
	delete flat_path;
	return bounding_box;
}

bool Path::try_select(const Gdiplus::PointF& cursor_pos, float margin, float scale) {
	float test_pen_width = pen->GetWidth() + margin / scale;
	Gdiplus::Pen test_pen(Gdiplus::Color(), test_pen_width);
	bool can_select = path->IsOutlineVisible(cursor_pos, &test_pen);
	return can_select;
}

void Path::transform(const Gdiplus::Matrix* transform) {
	path->Transform(transform);
}

std::shared_ptr<Element> Path::clone() {
	return std::make_shared<Path>(*this);
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
