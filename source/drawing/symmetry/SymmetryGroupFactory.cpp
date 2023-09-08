#include "SymmetryGroupFactory.h"
#include "TrivialGroup.h"
#include "WallpaperGroup.h"


std::shared_ptr<TrivialGroup> SymmetryGroupFactory::trivial() {
	return std::make_shared<TrivialGroup>();
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p1(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());	// identity transformation

	SymbolicPoint bottom_left ("(-v1_x - v2_x) / 2", "(-v1_y - v2_y) / 2");
	SymbolicPoint top_left    ("(-v1_x + v2_x) / 2", "(-v1_y + v2_y) / 2");
	SymbolicPoint bottom_right("( v1_x - v2_x) / 2", "( v1_y - v2_y) / 2");
	SymbolicPoint top_right   ("( v1_x + v2_x) / 2", "( v1_y + v2_y) / 2");
	WallpaperGroup::DomainBoundaries edges;
	edges.type_A.push_back({ bottom_left, top_left });
	edges.type_A.push_back({ bottom_right, top_right });
	edges.type_B.push_back({ bottom_left, bottom_right });
	edges.type_B.push_back({ top_left, top_right });

	return std::make_shared<WallpaperGroup>(
		"p1",
		WallpaperGroup::PARALLELOGRAM,
		cell,
		std::vector<SymbolicPoint>{},
		edges,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p2(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	SymbolicPoint center(0, 0);
	cell.push_back(SymbolicMatrix::rotate(180, center));

	SymbolicPoint bottom_left ("(-v1_x - v2_x) / 2", "(-v1_y - v2_y) / 2");
	SymbolicPoint center_left (" -v1_x         / 2", " -v1_y         / 2");
	SymbolicPoint top_left    ("(-v1_x + v2_x) / 2", "(-v1_y + v2_y) / 2");
	SymbolicPoint bottom_mid  ("        -v2_x  / 2", "        -v2_y  / 2");
	SymbolicPoint center_mid  (0, 0);
	SymbolicPoint top_mid     ("         v2_x  / 2", "         v2_y  / 2");
	SymbolicPoint bottom_right("( v1_x - v2_x) / 2", "( v1_y - v2_y) / 2");
	SymbolicPoint center_right("  v1_x         / 2", "  v1_y         / 2");
	SymbolicPoint top_right   ("( v1_x + v2_x) / 2", "( v1_y + v2_y) / 2");

	std::vector<SymbolicPoint> centers{
		top_left, top_mid, top_right,
		center_left, center_mid, center_right,
		bottom_left, bottom_mid, bottom_right,
	};

	WallpaperGroup::DomainBoundaries edges;
	edges.type_A.push_back({ bottom_left, center_left });
	edges.type_A.push_back({ top_left, center_left });
	edges.type_A.push_back({ bottom_right, center_right });
	edges.type_A.push_back({ top_right, center_right });
	edges.type_B.push_back({ bottom_left, bottom_mid });
	edges.type_B.push_back({ bottom_right, bottom_mid });
	edges.type_B.push_back({ top_left, top_mid });
	edges.type_B.push_back({ top_right, top_mid });
	edges.type_C.push_back({ center_left, center_mid });
	edges.type_C.push_back({ center_right, center_mid });

	return std::make_shared<WallpaperGroup>(
		"p2",
		WallpaperGroup::PARALLELOGRAM,
		cell,
		centers,
		edges,
		old
	);
}

//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pg(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::cm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pmm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pmg(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::pgg(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
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
		else {
			std::string error_message = "unknown wallpaper group " + name;
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

