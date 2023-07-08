#include <string>
#include "Path.h"
#include "Logger.h"

namespace Constellation {

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

	void Path::add_point(int xPos, int yPos) {
		control_points.push_back(Gdiplus::Point(xPos, yPos));
		++n_points;
	}
	
	void Path::add_points(Path& other) {
		// Add all but the first point
		for (auto point = ++other.control_points.begin(); point != other.control_points.end(); ++point) {
			control_points.push_back(*point);
			++n_points;
		}
	}

	void Path::pop_point() {
		control_points.pop_back();
		--n_points;
	}

}