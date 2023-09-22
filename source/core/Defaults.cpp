#include "Defaults.h"


Defaults::Defaults() {
	// new elements
	pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0), 1.0f);
	brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 255));

	// tool handles
	handle_pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0), 0.0f);
	handle_brush = new Gdiplus::SolidBrush(Gdiplus::Color(180, 255, 255, 255));
	selected_handle_brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 0, 0));

	// other tool stuff
	wip_pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 255, 255), 0.0f);
	tool_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 0, 0), 0.0f);
	symmetry_vector_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 255, 0), 0.0f);

	// symmetry guides
	domain_boundary_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 100, 0), 0.0f);
	drawing_area_brush = new Gdiplus::SolidBrush(Gdiplus::Color(100, 0, 80, 255));
	rotation_center_brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 100, 0));
}
