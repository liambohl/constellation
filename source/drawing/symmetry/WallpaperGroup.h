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
	enum shape {
		PARALLELOGRAM,
		RECTANGLE,
		RHOMBUS,
		HEXAGON,
		SQUARE
	};

	// This struct bundles the edges of all fundamental domains in a wallpaper group's central cell.
	// Edges that are equivalent under one of the symmetry group's transformations should have the same type.
	// Edges are drawn differently depending on their type (see defaults).
	struct DomainBoundaries {
		std::vector<SymbolicLine> mirror_lines;
		std::vector<SymbolicLine> type_A;
		std::vector<SymbolicLine> type_A_mirror;
		std::vector<SymbolicLine> type_B;
		std::vector<SymbolicLine> type_B_mirror;
		std::vector<SymbolicLine> type_C;
		std::vector<SymbolicLine> type_C_mirror;
	};

	WallpaperGroup(
		std::string name,
		shape cell_shape,
		std::vector<SymbolicMatrix> cell,
		std::vector<SymbolicPoint> rotation_centers,
		DomainBoundaries domain_boundaries,
		std::shared_ptr<SymmetryGroup> old
	);
	//WallpaperGroup();

	// Get the set of affine transforms necessary to tessalate a region.
	// The tiled region is a parallelogram of (2 * extent + 1) x (2 * extent + 1) cells.
	// Each matrix is an affine transformation that should be applied to each element of the drawing.
	std::vector<std::shared_ptr<Gdiplus::Matrix>> get_transforms() override { return transforms; }

	// Set v1 and ensure that v2 maintains this wallpaper group's shape
	void set_v1(float x, float y);
	// Set v2 and ensure that v1 maintains this wallpaper group's shape
	void set_v2(float x, float y);
	// Make this symmetry group draw more or fewer copies of its cell
	void set_extent(int extent);

	void draw(Gdiplus::Graphics* graphics, Defaults& defaults, float scale) override;

	json to_json() override;

private:
	// Calculate transforms based on current value of v1 and v2
	void update_transforms();

	// Draw the given boundary with the given shape
	void draw_boundary(
		Gdiplus::Graphics* graphics,
		Defaults& defaults,
		const SymbolicLine& boundary,
		const std::unordered_map<std::string, float>& variables,
		std::vector<Gdiplus::PointF> shape,
		bool mirrored=false
	);

	// coordinates of the vectors that form two edges of a cell
	float v1_x, v1_y, v2_x, v2_y;

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

	// Points around which this symmetry group has some rotational symmetry. Includes only centers inside, or on boundary of, the central cell
	std::vector<SymbolicPoint> rotation_centers;

	// Edges of all fundamental domains in the central cell
	DomainBoundaries domain_boundaries;
};
