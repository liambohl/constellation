#pragma once
#include "WallpaperGroup.h"

class SymmetryGroupFactory
{
public:
	static std::shared_ptr<SymmetryGroup> trivial();

	static std::shared_ptr<SymmetryGroup> p1(std::shared_ptr<SymmetryGroup> old);
	static std::shared_ptr<SymmetryGroup> p2(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> pm(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> pg(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> cm(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> pmm(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> pmg(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> pgg(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> cmm(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> p4(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> p4m(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> p4g(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> p3(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> p3m1(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> p31m(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> p6(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<SymmetryGroup> p6m(std::shared_ptr<SymmetryGroup> old);

private:
	// If changing from one wallpaper group to another,
	// update v1 and v2 in our new group to match those from our old group.
	// v1 should always match, and v2 should match length if possible and direction if possible
	static void match_vectors(std::shared_ptr<SymmetryGroup> old_group, std::shared_ptr<WallpaperGroup> new_group);
};

