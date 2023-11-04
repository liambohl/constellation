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
	delete temp_path;
	delete pen;
}

std::optional<Gdiplus::RectF> Path::get_bounding_box() {
	if (path->GetPointCount() == 0)
		return {};
	Gdiplus::GraphicsPath* flat_path;
	if (mid_transform)
		flat_path = temp_path->Clone();
	else
		flat_path = path->Clone();
	flat_path->Flatten();
	Gdiplus::RectF bounding_box;
	flat_path->GetBounds(&bounding_box, nullptr, pen);
	delete flat_path;
	return bounding_box;
}

bool Path::intersects_rectangle(Gdiplus::RectF& rectangle) {
	Gdiplus::GraphicsPath* footprint = path->Clone();	// This path, including its outline
	footprint->Widen(pen);
	Gdiplus::Region intersection(footprint);			// Overlap between this path and the rectangle
	intersection.Intersect(rectangle);

	Gdiplus::Bitmap blank_image(10, 10);
	Gdiplus::Graphics throwaway_graphics(&blank_image);
	return !intersection.IsEmpty(&throwaway_graphics);
}

bool Path::try_select(const Gdiplus::PointF& cursor_pos, float margin, float scale) {
	float test_pen_width = pen->GetWidth() + margin / scale;
	Gdiplus::Pen test_pen(Gdiplus::Color(), test_pen_width);
	bool can_select = path->IsOutlineVisible(cursor_pos, &test_pen);
	return can_select;
}

void Path::transform(const Gdiplus::Matrix* transform) {
	path->Transform(transform);
	mid_transform = false;
}

void Path::transform_temp(const Gdiplus::Matrix* transform) {
	temp_path = path->Clone();
	temp_path->Transform(transform);
	mid_transform = true;
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
	if (mid_transform)
		graphics->DrawPath(pen, temp_path);
	else
		graphics->DrawPath(pen, path);
}
