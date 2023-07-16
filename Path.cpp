#include <string>

#include "json_converters.h"
#include "Logger.h"
#include "Path.h"

namespace Constellation {

	Path::Path(json path_json):
		Element(path_json),
		pen(pen_from_json(path_json["pen"]))
	{
		for (json point_json : path_json["control_points"])
			control_points.emplace_back(point_from_json(point_json));
		n_points = control_points.size();
	}

	//Path::Path(const Path& other) :
	//	Element(other.id),
	//	control_points(other.control_points),
	//	n_points(other.n_points)
	//{
	//	pen = other.pen->Clone();
	//}

	Path::~Path() {
		delete pen;
	}

	void Path::draw(Canvas& canvas) {
		if (n_points < 4)
			return;

		Logger::get_instance()->log("Drawing path:");
		for (auto& point : control_points) {
			*Logger::get_instance() << '(' << point.X << ", " << point.Y << ')' << std::endl;
		}

		canvas.graphics->DrawBeziers(pen, control_points.data(), n_points);
	}

	json Path::to_json() {
		json output = {
			{"type", "Path"},
			{"id", id},
			{"pen", pen_to_json(pen)},
			{"control_points", {}}
		};

		// Populate control points
		for (Gdiplus::Point point : control_points) {
			output["control_points"].push_back(point_to_json(point));
		}

		return output;
	}

	void Path::add_point(int xPos, int yPos, int count) {
		for (int i = 0; i < count; ++i) {
			control_points.push_back(Gdiplus::Point(xPos, yPos));
			++n_points;
		}
	}
	
	void Path::add_points(Path& other) {
		// Add all but the first point
		for (auto point = ++other.control_points.begin(); point != other.control_points.end(); ++point) {
			control_points.push_back(*point);
			++n_points;
		}
	}

	void Path::pop_point(int count) {
		for (int i = 0; i < count; ++i) {
			control_points.pop_back();
			--n_points;
		}
	}

	Gdiplus::Point Path::top() {
		return control_points.back();
	}

}