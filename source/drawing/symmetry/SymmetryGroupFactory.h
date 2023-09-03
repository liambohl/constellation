#pragma once
#include "TrivialGroup.h"
#include "WallpaperGroup.h"

class SymmetryGroupFactory
{
public:
	static std::shared_ptr<TrivialGroup> trivial();

	static std::shared_ptr<WallpaperGroup> p1(std::shared_ptr<SymmetryGroup> old);
	static std::shared_ptr<WallpaperGroup> p2(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> pm(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> pg(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> cm(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> pmm(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> pmg(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> pgg(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> cmm(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> p4(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> p4m(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> p4g(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> p3(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> p3m1(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> p31m(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> p6(std::shared_ptr<SymmetryGroup> old);
	//static std::shared_ptr<WallpaperGroup> p6m(std::shared_ptr<SymmetryGroup> old);

	static std::shared_ptr<SymmetryGroup> from_json(json symm_json);
};

