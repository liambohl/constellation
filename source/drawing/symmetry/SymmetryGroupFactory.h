#pragma once
#include "WallpaperGroup.h"

class SymmetryGroupFactory
{
public:
	static SymmetryGroup* trivial();

	static SymmetryGroup* p1(SymmetryGroup* old);
	static SymmetryGroup* p2(SymmetryGroup* old);
	//static SymmetryGroup* pm(SymmetryGroup* old);
	//static SymmetryGroup* pg(SymmetryGroup* old);
	//static SymmetryGroup* cm(SymmetryGroup* old);
	//static SymmetryGroup* pmm(SymmetryGroup* old);
	//static SymmetryGroup* pmg(SymmetryGroup* old);
	//static SymmetryGroup* pgg(SymmetryGroup* old);
	//static SymmetryGroup* cmm(SymmetryGroup* old);
	//static SymmetryGroup* p4(SymmetryGroup* old);
	//static SymmetryGroup* p4m(SymmetryGroup* old);
	//static SymmetryGroup* p4g(SymmetryGroup* old);
	//static SymmetryGroup* p3(SymmetryGroup* old);
	//static SymmetryGroup* p3m1(SymmetryGroup* old);
	//static SymmetryGroup* p31m(SymmetryGroup* old);
	//static SymmetryGroup* p6(SymmetryGroup* old);
	//static SymmetryGroup* p6m(SymmetryGroup* old);

private:
	// If changing from one wallpaper group to another,
	// update v1 and v2 in our new group to match those from our old group.
	// v1 should always match, and v2 should match length if possible and direction if possible
	static void match_vectors(SymmetryGroup* old_group, WallpaperGroup* new_group);
};

