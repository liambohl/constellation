#pragma once

#include "core/framework.h"
#include "math/SymbolicPoint.h"


class Defaults
{
public:
	Defaults();

	Gdiplus::Color background_color;					// "transparent" color shown where there are no elements

	// new elements
	Gdiplus::Pen* pen = nullptr;						// default stroke for elements in the drawing
	Gdiplus::Brush* brush = nullptr;					// default fill for elements in the drawing
	
	// tools
	Gdiplus::Pen* wip_pen = nullptr;					// works in progress: elements currently being created by a tool
	Gdiplus::Pen* tool_pen = nullptr;					// the cutting edge
	Gdiplus::Pen* symmetry_vector_pen = nullptr;		// vectors that parameterize a symmetry group
	Gdiplus::Pen* selection_pen_1 = nullptr;			// bounding rectangle of selection; bottom layer
	Gdiplus::Pen* selection_pen_2 = nullptr;			// bounding rectangle of selection; top layer

	// symmetry guides
	Gdiplus::Pen* domain_boundary_pen = nullptr;		// edges of symmetry domains
	Gdiplus::Brush* drawing_area_brush = nullptr;		// area in which the user should draw for the best tiling
	Gdiplus::Brush* rotation_center_brush = nullptr;	// dots at rotation centers
	float rotation_center_radius = 3.0f;
};
