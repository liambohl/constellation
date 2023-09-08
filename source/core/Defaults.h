#pragma once
#include "core/framework.h"
#include "math/SymbolicPoint.h"

class Defaults
{
public:
	// new elements
	Gdiplus::Pen* pen = nullptr;						// default stroke for elements in the drawing
	Gdiplus::Brush* brush = nullptr;					// default fill for elements in the drawing

	// tools
	Gdiplus::Pen* wip_pen = nullptr;					// works in progress: elements currently being created by a tool
	Gdiplus::Pen* tool_pen = nullptr;					// the cutting edge

	// symmetry guides
	Gdiplus::Pen* domain_boundary_pen = nullptr;		// edges of symmetry domains
	Gdiplus::Brush* symmetry_cell_brush = nullptr;		// symmetry cell guide - marks the area where the user should draw
	Gdiplus::Brush* rotation_center_brush = nullptr;	// dots at rotation centers
	float rotation_center_radius = 3.0f;

	// symmetry guides - domain boundary shapes
	// Each of these represents a Bezier curve from (0, 0) to (1, 0).
	std::vector<Gdiplus::PointF> boundary_shape_A;		// bump out
	std::vector<Gdiplus::PointF> boundary_shape_B;		// zig zag out
	std::vector<Gdiplus::PointF> boundary_shape_C;		// puzzle piece key

	Defaults();
};
