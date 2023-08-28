#include "SymmetryGroupFactory.h"
#include "TrivialGroup.h"
#include "WallpaperGroup.h"
//#include "math/SymbolicPoint.h"


std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::trivial() {
	return std::make_shared<TrivialGroup>();
}

std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p1(std::shared_ptr<SymmetryGroup> old) {
	std::vector<SymbolicMatrix> cell;
	cell.push_back(SymbolicMatrix());

	return std::make_shared<WallpaperGroup>(
		"p1",
		WallpaperGroup::PARALLELOGRAM,
		cell,
		old
	);
}

std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p2(std::shared_ptr<SymmetryGroup> old) {
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

//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::pm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::pg(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::cm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::pmm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::pmg(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::pgg(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::cmm(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p4(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p4m(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p4g(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p3(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p3m1(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p31m(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p6(std::shared_ptr<SymmetryGroup> old) {
//
//}
//
//std::shared_ptr<SymmetryGroup> SymmetryGroupFactory::p6m(std::shared_ptr<SymmetryGroup> old) {
//
//}

void SymmetryGroupFactory::match_vectors(std::shared_ptr<SymmetryGroup> old_group, std::shared_ptr<WallpaperGroup> new_group) {
	if (std::shared_ptr<WallpaperGroup> old_wallpaper = dynamic_pointer_cast<WallpaperGroup>(old_group)) {
		new_group->set_v2(old_wallpaper->v2_x, old_wallpaper->v2_y);
		new_group->set_v1(old_wallpaper->v1_x, old_wallpaper->v1_y);
	}
}
