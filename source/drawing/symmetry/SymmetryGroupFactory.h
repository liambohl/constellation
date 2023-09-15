#pragma once
#include "TrivialGroup.h"
#include "WallpaperGroup.h"

class SymmetryGroupFactory
{
public:
	static SymmetryGroupFactory& get_instance();

	std::shared_ptr<TrivialGroup> trivial();

	std::shared_ptr<WallpaperGroup> p1(std::shared_ptr<SymmetryGroup> old);
	std::shared_ptr<WallpaperGroup> p2(std::shared_ptr<SymmetryGroup> old);
	std::shared_ptr<WallpaperGroup> pm(std::shared_ptr<SymmetryGroup> old);
	std::shared_ptr<WallpaperGroup> pg(std::shared_ptr<SymmetryGroup> old);
	std::shared_ptr<WallpaperGroup> cm(std::shared_ptr<SymmetryGroup> old);
	std::shared_ptr<WallpaperGroup> pmm(std::shared_ptr<SymmetryGroup> old);
	std::shared_ptr<WallpaperGroup> pmg(std::shared_ptr<SymmetryGroup> old);
	std::shared_ptr<WallpaperGroup> pgg(std::shared_ptr<SymmetryGroup> old);
	std::shared_ptr<WallpaperGroup> cmm(std::shared_ptr<SymmetryGroup> old);
	//std::shared_ptr<WallpaperGroup> p4(std::shared_ptr<SymmetryGroup> old);
	//std::shared_ptr<WallpaperGroup> p4m(std::shared_ptr<SymmetryGroup> old);
	//std::shared_ptr<WallpaperGroup> p4g(std::shared_ptr<SymmetryGroup> old);
	//std::shared_ptr<WallpaperGroup> p3(std::shared_ptr<SymmetryGroup> old);
	//std::shared_ptr<WallpaperGroup> p3m1(std::shared_ptr<SymmetryGroup> old);
	//std::shared_ptr<WallpaperGroup> p31m(std::shared_ptr<SymmetryGroup> old);
	//std::shared_ptr<WallpaperGroup> p6(std::shared_ptr<SymmetryGroup> old);
	//std::shared_ptr<WallpaperGroup> p6m(std::shared_ptr<SymmetryGroup> old);

	std::shared_ptr<SymmetryGroup> from_json(json symm_json);

private:
	SymmetryGroupFactory();

	static SymmetryGroupFactory* instance;

	// vertices for parallelogram cells
	// For cm and cmm, bottom_left is actually on the left of the diagram.
	// I named these points thinking that the y coordinate increases moving *up* the screen, but that's backward. Oh well.
	SymbolicPoint bottom_left;
	SymbolicPoint center_left;
	SymbolicPoint top_left;
	SymbolicPoint bottom_mid;
	SymbolicPoint origin;
	SymbolicPoint top_mid;
	SymbolicPoint bottom_right;
	SymbolicPoint center_right;
	SymbolicPoint top_right;

	// midlines for parallelogram cells
	SymbolicLine horizontal_midline;
	SymbolicLine vertical_midline;
};

