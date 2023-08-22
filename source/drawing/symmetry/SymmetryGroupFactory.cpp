#include "SymmetryGroupFactory.h"
#include "TrivialGroup.h"
#include "WallpaperGroup.h"
//#include "math/SymbolicPoint.h"


SymmetryGroup* SymmetryGroupFactory::trivial() {
	return new TrivialGroup;
}

SymmetryGroup* SymmetryGroupFactory::p1(SymmetryGroup* old) {
	WallpaperGroup* group = new WallpaperGroup("p1");
	group->cell_shape = WallpaperGroup::PARALLELOGRAM;
	match_vectors(old, group);
	return group;
}

SymmetryGroup* SymmetryGroupFactory::p2(SymmetryGroup* old) {
	WallpaperGroup* group = new WallpaperGroup("p2");
	group->cell_shape = WallpaperGroup::PARALLELOGRAM;
	SymbolicPoint center("(v1_x + v2_x) / 2", "(v1_y + v2_y) / 2");
	group->cell.push_back(SymbolicMatrix::rotate(180, center));
	match_vectors(old, group);
	return group;
}

//SymmetryGroup* SymmetryGroupFactory::pm(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::pg(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::cm(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::pmm(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::pmg(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::pgg(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::cmm(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::p4(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::p4m(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::p4g(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::p3(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::p3m1(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::p31m(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::p6(SymmetryGroup* old) {
//
//}
//
//SymmetryGroup* SymmetryGroupFactory::p6m(SymmetryGroup* old) {
//
//}

void SymmetryGroupFactory::match_vectors(SymmetryGroup* old_group, WallpaperGroup* new_group) {
	if (WallpaperGroup* old_wallpaper = dynamic_cast<WallpaperGroup*>(old_group)) {
		new_group->set_v2(old_wallpaper->v2_x, old_wallpaper->v2_y);
		new_group->set_v1(old_wallpaper->v1_x, old_wallpaper->v1_y);
	}
}
