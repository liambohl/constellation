#pragma once

#include <vector>

#include "framework.h"

class WallpaperGroup {
public:
	static WallpaperGroup p1();
	static WallpaperGroup p2();
	static WallpaperGroup pm();
	static WallpaperGroup pg();
	static WallpaperGroup cm();
	static WallpaperGroup pmm();
	static WallpaperGroup pmg();
	static WallpaperGroup pgg();
	static WallpaperGroup cmm();
	static WallpaperGroup p4();
	static WallpaperGroup p4m();
	static WallpaperGroup p4g();
	static WallpaperGroup p3();
	static WallpaperGroup p3m1();
	static WallpaperGroup p31m();
	static WallpaperGroup p6();
	static WallpaperGroup p6m();

	// Get the set of affine transforms necessary to fill the window with copies
	// of the fundamental domain, each copy transformed by one of the affine transforms.
	std::vector<Gdiplus::Matrix> fill_window(Gdiplus::RectF& window);

private:
	enum shape {
		PARALLELOGRAM,	// any parallelogram
		RECTANGLE,
		RHOMBUS,		// any rhombus
		DIAMOND,		// rhombus with 60° and 120° angles
		SQUARE
	};

	WallpaperGroup(enum shape wallpaper_unit_shape, std::vector<Gdiplus::Matrix> wallpaper_unit) :
		wallpaper_unit_shape(wallpaper_unit_shape),
		wallpaper_unit(wallpaper_unit)
	{
		wallpaper_unit.push_back(Gdiplus::Matrix());	// Always include the identity
	}

	// After drawing one or more rotated, mirrored, and glide-mirrored
	// copies of the fundamental domain to form a wallpaper unit,
	// the plane can be "wallpapered" by drawing copies of that wallpaper unit,
	// translated by integer linear combinations of these vectors.
	Gdiplus::PointF v1 = Gdiplus::PointF(100.0f, 0.0f);
	Gdiplus::PointF v2 = Gdiplus::PointF(0.0f, 100.0f);

	// Does this wallpaper group require the wallpaper unit to be a particular shape?
	enum shape wallpaper_unit_shape;

	// A "wallpaper unit" can be drawn by drawing one or more copies of the fundamental domain,
	// each transformed by one of these affine transformations.
	// This set always includes the identity.
	std::vector<Gdiplus::Matrix> wallpaper_unit;
};