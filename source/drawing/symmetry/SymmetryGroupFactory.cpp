#include "SymmetryGroupFactory.h"
#include "TrivialGroup.h"
#include "WallpaperGroup.h"
//#include "math/SymbolicPoint.h"


std::shared_ptr<TrivialGroup> SymmetryGroupFactory::trivial() {
	return std::make_shared<TrivialGroup>();
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p1(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());

	return std::make_shared<WallpaperGroup>(
		"p1",
		WallpaperGroup::PARALLELOGRAM,
		cell,
		old
	);
}

std::shared_ptr<WallpaperGroup> SymmetryGroupFactory::p2(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());
	SymbolicPoint center("(v1_x + v2_x) / 2", "(v1_y + v2_y) / 2");
	cell.push_back(SymbolicMatrix::rotate(180, center));

	return std::make_shared<WallpaperGroup>(
		"p1",
		WallpaperGroup::PARALLELOGRAM,
		cell,
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

