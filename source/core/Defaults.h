#pragma once

#include "core/framework.h"
#include "math/SymbolicPoint.h"


class Defaults
{
public:
	Defaults();

	// new elements
	Gdiplus::Pen* pen = nullptr;						// default stroke for elements in the drawing
	Gdiplus::Brush* brush = nullptr;					// default fill for elements in the drawing

	// tool handles
	Gdiplus::Pen* handle_pen = nullptr;					// small handles that can be dragged to edit something
	float handle_radius = 5.0f;
	Gdiplus::Brush* handle_brush = nullptr;
	Gdiplus::Brush* selected_handle_brush = nullptr;	// fill the handle under the cursor
	float selection_range_squared = 100.0f;             // pixels from center of handle to cursor to allow selection, squared for efficiently comparing distances)
	
	// other tool stuff
	Gdiplus::Pen* wip_pen = nullptr;					// works in progress: elements currently being created by a tool
	Gdiplus::Pen* tool_pen = nullptr;					// the cutting edge
	Gdiplus::Pen* symmetry_vector_pen = nullptr;		// vectors that parameterize a symmetry group

	// symmetry guides
	Gdiplus::Pen* domain_boundary_pen = nullptr;		// edges of symmetry domains
	Gdiplus::Brush* drawing_area_brush = nullptr;		// area in which the user should draw for the best tiling
	Gdiplus::Brush* rotation_center_brush = nullptr;	// dots at rotation centers
	float rotation_center_radius = 3.0f;
};
