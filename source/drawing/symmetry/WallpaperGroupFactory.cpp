#include "WallpaperGroupFactory.h"
//#include "math/SymbolicPoint.h"

WallpaperGroup WallpaperGroupFactory::p1() {
	WallpaperGroup group;
	group.cell_shape = WallpaperGroup::PARALLELOGRAM;
	return group;
}

WallpaperGroup WallpaperGroupFactory::p2() {
	WallpaperGroup group;
	group.cell_shape = WallpaperGroup::PARALLELOGRAM;
	SymbolicPoint center("(v1_x + v2_x) / 2", "(v1_y + v2_y) / 2");
	group.cell.push_back(SymbolicMatrix::rotate(180, center));
	return group;
}

//WallpaperGroup WallpaperGroupFactory::pm() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::pg() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::cm() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::pmm() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::pmg() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::pgg() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::cmm() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::p4() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::p4m() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::p4g() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::p3() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::p3m1() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::p31m() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::p6() {
//
//}
//
//WallpaperGroup WallpaperGroupFactory::p6m() {
//
//}