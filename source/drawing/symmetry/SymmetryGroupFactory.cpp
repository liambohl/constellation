#include "SymmetryGroupFactory.h"

#include <iomanip>

#include "TrivialGroup.h"
#include "WallpaperGroup.h"
#include "core/Logger.h"


std::ostream& operator<<(std::ostream& os, Gdiplus::Matrix& matrix) {
	float elements[6];
	matrix.GetElements(elements);
	os << "[   [ " << std::setw(4) << elements[0] << " " << std::setw(4) << elements[1] << "    0 ]" << std::endl;
	os << "    [ " << std::setw(4) << elements[2] << " " << std::setw(4) << elements[3] << "    0 ]" << std::endl;
	os << "    [ " << std::setw(4) << elements[4] << " " << std::setw(4) << elements[5] << "    1 ]" << std::endl;
	return os;
}

SymmetryGroupFactory* SymmetryGroupFactory::instance = nullptr;

SymmetryGroupFactory& SymmetryGroupFactory::get_instance() {
	if (instance == nullptr)
		instance = new SymmetryGroupFactory();
	return *instance;
}

SymmetryGroupFactory::SymmetryGroupFactory() {
	// vertices for parallelogram cells
	bottom_left  = { "(-v1_x - v2_x) / 2", "(-v1_y - v2_y) / 2" };
	center_left  = { " -v1_x         / 2", " -v1_y         / 2" };
	top_left     = { "(-v1_x + v2_x) / 2", "(-v1_y + v2_y) / 2" };
	bottom_mid   = { "        -v2_x  / 2", "        -v2_y  / 2" };
	origin       = { 0, 0 };
	top_mid      = { "         v2_x  / 2", "         v2_y  / 2" };
	bottom_right = { "( v1_x - v2_x) / 2", "( v1_y - v2_y) / 2" };
	center_right = { "  v1_x         / 2", "  v1_y         / 2" };
	top_right    = { "( v1_x + v2_x) / 2", "( v1_y + v2_y) / 2" };

	// midlines for parallelogram cells
	horizontal_midline = { center_left, center_right };
	vertical_midline = { bottom_mid, top_mid };
}

std::shared_ptr<TrivialGroup> SymmetryGroupFactory::trivial() {
	return std::make_shared<TrivialGroup>();
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p1(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());	// identity transformation

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, top_left });
	edges.add_type_A({ bottom_right, top_right });
	edges.add_type_B({ bottom_left, bottom_right });
	edges.add_type_B({ top_left, top_right });

	return std::make_shared<WallpaperGroup>(
		"p1",
		WallpaperGroup::PARALLELOGRAM,
		cell,
		drawing_area,
		std::vector<SymbolicPoint>{},
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p2(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::rotate(180, origin));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
			center_left, origin, center_right,
			bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ top_left, center_left });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A({ top_right, center_right });
	edges.add_type_B({ bottom_left, bottom_mid });
	edges.add_type_B({ bottom_right, bottom_mid });
	edges.add_type_B({ top_left, top_mid });
	edges.add_type_B({ top_right, top_mid });
	edges.add_type_C({ center_left, origin });
	edges.add_type_C({ center_right, origin });

	return std::make_shared<WallpaperGroup>(
		"p2",
		WallpaperGroup::PARALLELOGRAM,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pm(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::reflect(horizontal_midline));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, bottom_right });
	edges.add_mirror_line({ center_left, center_right });
	edges.add_mirror_line({ top_left, top_right });
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A_mirror({ top_left, center_left });
	edges.add_type_A_mirror({ top_right, center_right });

	return std::make_shared<WallpaperGroup>(
		"pm",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		std::vector<SymbolicPoint>{},
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pg(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::reflect(vertical_midline) * SymbolicMatrix::translate(top_mid));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, center_left, center_right, bottom_right
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, bottom_right });
	edges.add_type_A({ top_left, top_right });
	edges.add_type_A_mirror({ center_right, center_left });
	edges.add_type_B({ bottom_left, center_left });
	edges.add_type_B({ bottom_right, center_right });
	edges.add_type_B_mirror({ center_left, top_left });
	edges.add_type_B_mirror({ center_right, top_right });

	return std::make_shared<WallpaperGroup>(
		"pg",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		std::vector<SymbolicPoint>{},
		edges,
		old
	);
}

//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::cm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pmm(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(SymbolicMatrix::reflect(vertical_midline));
	cell.push_back(SymbolicMatrix::reflect(horizontal_midline));
	cell.push_back(SymbolicMatrix::reflect(vertical_midline) * SymbolicMatrix::reflect(horizontal_midline));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, top_left, top_right, bottom_right
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
			center_left, origin, center_right,
			bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, bottom_right });
	edges.add_mirror_line({ center_left, center_right });
	edges.add_mirror_line({ top_left, top_right });
	edges.add_mirror_line({ bottom_left, top_left });
	edges.add_mirror_line({ bottom_mid, top_mid });
	edges.add_mirror_line({ bottom_right, top_right });

	return std::make_shared<WallpaperGroup>(
		"pmm",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pmg(std::shared_ptr<SymmetryGroup> old) {
	SymbolicMatrix vertical_glide = SymbolicMatrix::reflect(vertical_midline) * SymbolicMatrix::translate(top_mid);
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(vertical_glide);
	cell.push_back(SymbolicMatrix::reflect(horizontal_midline));
	cell.push_back(vertical_glide * SymbolicMatrix::reflect(horizontal_midline));

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, center_left, center_right, bottom_right
	};

	SymbolicPoint one_quarter_left    = { "-v1_x / 2 - v2_x / 4", "-v1_y / 2 - v2_y / 4" };	// one quarter of the way from bottom left to top left
	SymbolicPoint three_quarter_left  = { "-v1_x / 2 + v2_x / 4", "-v1_y / 2 + v2_y / 4" };
	SymbolicPoint one_quarter_mid     = { "           -v2_x / 4", "           -v2_y / 4" };
	SymbolicPoint three_quarter_mid   = { "            v2_x / 4", "            v2_y / 4" };
	SymbolicPoint one_quarter_right   = { " v1_x / 2 - v2_x / 4", " v1_y / 2 - v2_y / 4" };
	SymbolicPoint three_quarter_right = { " v1_x / 2 + v2_x / 4", " v1_y / 2 + v2_y / 4" };

	std::vector<SymbolicPoint> centers {
		three_quarter_left, three_quarter_mid, three_quarter_right,
			one_quarter_left, one_quarter_mid, one_quarter_right,
	};

	DomainBoundaries edges;
	edges.add_mirror_line({ bottom_left, bottom_right });
	edges.add_mirror_line({ center_left, center_right });
	edges.add_mirror_line({ top_left, top_right });
	edges.add_type_A({ bottom_left, one_quarter_left });
	edges.add_type_A({ center_left, one_quarter_left });
	edges.add_type_A({ bottom_right, one_quarter_right });
	edges.add_type_A({ center_right, one_quarter_right });
	edges.add_type_A_mirror({ top_left, three_quarter_left });
	edges.add_type_A_mirror({ center_left, three_quarter_left });
	edges.add_type_A_mirror({ top_right, three_quarter_right });
	edges.add_type_A_mirror({ center_right, three_quarter_right });
	edges.add_type_B({ bottom_mid, one_quarter_mid });
	edges.add_type_B({ origin, one_quarter_mid });
	edges.add_type_B_mirror({ top_mid, three_quarter_mid });
	edges.add_type_B_mirror({ origin, three_quarter_mid });

	return std::make_shared<WallpaperGroup>(
		"pmg",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pgg(std::shared_ptr<SymmetryGroup> old) {
	SymbolicPoint one_quarter_left   = { "-v1_x / 2 - v2_x / 4", "-v1_y / 2 - v2_y / 4" };	// one quarter of the way from bottom left to top left
	SymbolicPoint one_quarter_mid    = { "           -v2_x / 4", "           -v2_y / 4" };

	auto horizontal_mirror = SymbolicMatrix::reflect({ one_quarter_left, one_quarter_mid });
	auto horizontal_translate = SymbolicMatrix::translate(center_right);
	SymbolicMatrix horizontal_glide = horizontal_mirror * horizontal_translate;
	SymbolicMatrix rotate_180 = SymbolicMatrix::rotate(180, origin);

	//std::unordered_map<std::string, float> variables = { {"v1_x", 1.0f}, {"v1_y", 0.0f}, {"v2_x", 0.0f}, {"v2_y", 1.0f} };
	//*Logger::get_instance() << "horizontal mirror:" << std::endl << horizontal_mirror.evaluate(variables) << std::endl;
	//*Logger::get_instance() << "horizontal translate:" << std::endl << horizontal_translate.evaluate(variables) << std::endl;
	//*Logger::get_instance() << "horizontal glide:" << std::endl << horizontal_glide.evaluate(variables) << std::endl;

	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	cell.push_back(horizontal_glide);
	cell.push_back(rotate_180);
	cell.push_back(horizontal_glide * rotate_180);

	std::vector<SymbolicPoint> drawing_area = {
		bottom_left, center_left, origin, bottom_mid
	};

	std::vector<SymbolicPoint> centers {
		top_left, top_mid, top_right,
			center_left, origin, center_right,
			bottom_left, bottom_mid, bottom_right,
	};

	DomainBoundaries edges;
	edges.add_type_A({ bottom_left, center_left });
	edges.add_type_A({ top_left, center_left });
	edges.add_type_A({ bottom_right, center_right });
	edges.add_type_A({ top_right, center_right });
	edges.add_type_A_mirror({ origin, bottom_mid });
	edges.add_type_A_mirror({ origin, top_mid });
	edges.add_type_B({ bottom_left, bottom_mid });
	edges.add_type_B({ bottom_right, bottom_mid });
	edges.add_type_B({ top_left, top_mid });
	edges.add_type_B({ top_right, top_mid });
	edges.add_type_B_mirror({ origin, center_left });
	edges.add_type_B_mirror({ origin, center_right });

	return std::make_shared<WallpaperGroup>(
		"pgg",
		WallpaperGroup::RECTANGLE,
		cell,
		drawing_area,
		centers,
		edges,
		old
	);

}

//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::cmm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p4(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p4m(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p4g(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p3(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p3m1(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p31m(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p6(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p6m(std::shared_ptr<SymmetryGroup> old) {
//
//}

std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::from_json(json symm_json) {
	std::string type = symm_json["type"];
	if (type == "TrivialGroup") {
		return trivial();
	}
	if (type == "WallpaperGroup") {
		std::shared_ptr<WallpaperGroup> wallpaper_group;
		std::string name = symm_json["name"];	// Which of the seventeen wallpaper groups?
		if (name == "p1")
			wallpaper_group = p1(nullptr);
		else if (name == "p2")
			wallpaper_group = p2(nullptr);
		else if (name == "pm")
			wallpaper_group = pm(nullptr);
		else if (name == "pg")
			wallpaper_group = pg(nullptr);
		else if (name == "pmm")
			wallpaper_group = pmm(nullptr);
		else if (name == "pmg")
			wallpaper_group = pmg(nullptr);
		else if (name == "pgg")
			wallpaper_group = pgg(nullptr);
		else {
			std::string error_message = "error: unknown wallpaper group " + name;
			throw std::exception(error_message.c_str());
		}
		wallpaper_group->set_v1(symm_json["v1_x"], symm_json["v1_y"]);
		wallpaper_group->set_v2(symm_json["v2_x"], symm_json["v2_y"]);
		wallpaper_group->set_extent(symm_json["extent"]);
		return wallpaper_group;
	}
	else {
		std::string error_message = "unknown symmetry group type " + type;
		throw std::exception(error_message.c_str());
	}
}

