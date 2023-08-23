#pragma once

#include <vector>

#include "SymmetryGroup.h"
#include "core/framework.h"
#include "math/SymbolicMatrix.h"

// Represents a symmetry group called a wallpaper group.
// We can tile a region of the plane using a wallpaper group in two steps:
// First, draw one or more copies of the fundamental domain to build a parallelogram-shaped cell.
// Then, draw one or more cells, each translated by an integer linear combination of v1 and v2, to tile a region
class WallpaperGroup :
	public SymmetryGroup
{
public:
	WallpaperGroup(std::string name) : SymmetryGroup(name) {}

	// Get the set of affine transforms necessary to tessalate a region.
	// The tiled region is a parallelogram of (2 * extent + 1) x (2 * extent + 1) cells.
	// Each matrix is an affine transformation that should be applied to each element of the drawing.
	std::vector<Gdiplus::Matrix*> get_transforms() override { return transforms; }

	// Set v1 and ensure that v2 maintains this wallpaper group's shape
	void set_v1(float x, float y);
	// Set v2 and ensure that v1 maintains this wallpaper group's shape
	void set_v2(float x, float y);
	// Make this symmetry group draw more or fewer copies of its cell
	void set_extent(int extent);

private:
	enum shape {
		PARALLELOGRAM,
		RECTANGLE,
		RHOMBUS,
		DIAMOND,		// rhombus with 60° and 120° angles
		SQUARE
	};

	void update_transforms();

	// After drawing one or more rotated, mirrored, and glide-mirrored
	// copies of the fundamental domain to form a cell,
	// the plane can be "wallpapered" by drawing copies of that cell,
	// translated by integer linear combinations of these vectors.
	// TODO: ?

	// coordinates of the vectors that form two edges of a cell
	float v1_x = 1.0f, v1_y = 0.0f, v2_x = 0.0f, v2_y = 1.0f;

	// how many times, in both the positive and negative direction, to copy the cell
	int extent = 3;

	// Constraint on the shape of this wallpaper group's cell
	enum shape cell_shape = PARALLELOGRAM;

	// A cell can be drawn by drawing one or more copies of the fundamental domain,
	// each transformed by one of these affine transformations.
	// This set always includes the identity.
	std::vector<SymbolicMatrix> cell = { SymbolicMatrix() };

	// Cached vector of concrete transforms
	std::vector<Gdiplus::Matrix*> transforms;

	friend class SymmetryGroupFactory;
};
