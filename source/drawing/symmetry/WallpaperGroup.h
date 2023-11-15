#pragma once

#include "SymmetryGroup.h"

#include <vector>

#include "DomainBoundaries.h"
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
	enum shape {
		PARALLELOGRAM,
		RECTANGLE,
		RHOMBUS,
		HEXAGON,
		SQUARE
	};

	WallpaperGroup(
		std::string name,
		shape cell_shape,
		std::vector<SymbolicMatrix> cell,
		std::vector<SymbolicPoint> drawing_area,
		std::vector<SymbolicPoint> rotation_centers,
		DomainBoundaries domain_boundaries,
		std::shared_ptr<SymmetryGroup> old
	);

	// Get the set of affine transforms necessary to tessalate a region.
	// The tiled region is a parallelogram of (2 * extent + 1) x (2 * extent + 1) cells.
	// Each matrix is an affine transformation that should be applied to each element of the drawing.
	std::vector<std::shared_ptr<Gdiplus::Matrix>> get_transforms() override { return transforms; }

	// Set v1 and ensure that v2 maintains this wallpaper group's shape
	void set_v1(Gdiplus::PointF v);
	// Set v2 and ensure that v1 maintains this wallpaper group's shape
	void set_v2(Gdiplus::PointF v);

	Gdiplus::PointF get_v1() { return v1; }
	Gdiplus::PointF get_v2() { return v2; }

	// Make this symmetry group draw more or fewer copies of its cell
	void set_extent(int extent);

	void draw(Gdiplus::Graphics* graphics, Defaults& defaults, float scale) override;

	json to_json() override;

private:
	// Calculate transforms based on current value of v1 and v2
	void update_transforms();

	// coordinates of the vectors that describe how the cell can be copied to tile the plane
	Gdiplus::PointF v1, v2;

	// how many times, in both the positive and negative direction, to copy the cell
	int extent = 2;

	// Constraint on the shape of this wallpaper group's cell
	enum shape cell_shape = PARALLELOGRAM;

	// A cell can be drawn by drawing one or more copies of the fundamental domain,
	// each transformed by one of these affine transformations.
	// This set should always include the identity.
	std::vector<SymbolicMatrix> cell;

	// Cached vector of concrete transforms
	std::vector<std::shared_ptr<Gdiplus::Matrix>> transforms;

	// Vertices of a polygon inside which the user should draw elements.
	// Drawing outside this area results in elements extending beyond or not reaching the edge of the tessalation.
	std::vector<SymbolicPoint> drawing_area;

	// Cached concrete drawing area
	std::vector<Gdiplus::PointF> drawing_area_evaluated;

	// Points around which this symmetry group has some rotational symmetry. Includes only centers inside, or on boundary of, the central cell
	std::vector<SymbolicPoint> rotation_centers;

	// Edges of all fundamental domains in the central cell
	DomainBoundaries domain_boundaries;
};
