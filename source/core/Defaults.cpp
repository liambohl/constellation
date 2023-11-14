#include "Defaults.h"

#include "gdiplusenums.h"

using namespace Gdiplus;


Defaults::Defaults() {
	background_color = Color(255, 80, 80, 80);

	// new elements
	pen = new Pen(Color(255, 0, 0, 0), 5.0f);
	pen->SetStartCap(LineCapRound);
	pen->SetEndCap(LineCapRound);
	pen->SetLineJoin(LineJoinRound);
	brush = new SolidBrush(Color(255, 255, 255, 255));

	// tools
	wip_pen = new Pen(Color(255, 0, 255, 255), 0.0f);
	tool_pen = new Pen(Color(255, 255, 0, 0), 0.0f);
	tangent_pen = new Pen(Color(255, 255, 120, 255), 0.0f);
	symmetry_vector_pen = new Pen(Color(255, 255, 255, 0), 0.0f);
	selection_pen_1 = new Pen(Color(255, 0, 0, 0), 0.0f);
	selection_pen_2 = new Pen(Color(255, 255, 255, 255), 0.0f);
	const float dash_array[2] = { 3.0f, 3.0f };
	selection_pen_2->SetDashPattern(dash_array, 2);
	ltr_selection_rectangle_pen = new Pen(Color(255, 150, 150, 150), 0.0f);
	rtl_selection_rectangle_pen = new Pen(Color(255, 255, 125, 50), 0.0f);
	ltr_selection_rectangle_brush = new SolidBrush(Color(50, 150, 150, 150));
	rtl_selection_rectangle_brush = new SolidBrush(Color(50, 255, 125, 50));

	// symmetry guides
	domain_boundary_pen = new Pen(Color(255, 255, 100, 0), 0.0f);
	drawing_area_brush = new SolidBrush(Color(100, 0, 80, 255));
	rotation_center_brush = new SolidBrush(Color(255, 255, 100, 0));
}
