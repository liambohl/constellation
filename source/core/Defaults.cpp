#include "Defaults.h"

Defaults::Defaults() {
	// new elements
	pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 0, 0), 1.0f);
	brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255, 255));

	// tools
	wip_pen = new Gdiplus::Pen(Gdiplus::Color(255, 0, 255, 255), 0.0f);
	tool_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 0, 0), 0.0f);

	// symmetry guides
	domain_boundary_pen = new Gdiplus::Pen(Gdiplus::Color(255, 255, 100, 0), 0.0f);
	symmetry_cell_brush = new Gdiplus::SolidBrush(Gdiplus::Color(100, 0, 80, 255));
	rotation_center_brush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 100, 0));

	// symmetry guides - domain boundary shapes
	boundary_shape_A = {
		{ 0.0f, 0.0f },		// point
		{ 0.0f, 0.0f },
		{ 0.25f, 0.0f },
		{ 0.25f, 0.0f },	// point
		{ 0.3f, 0.15f },
		{ 0.5f, 0.15f },
		{ 0.55f, 0.0f },	// point
		{ 0.55f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f }		// point
	};

	boundary_shape_B = {
		{ 0.0f, 0.0f },		// point
		{ 0.0f, 0.0f },
		{ 0.45f, 0.0f },
		{ 0.45f, 0.0f },	// point
		{ 0.45f, 0.0f },
		{ 0.6f, 0.15f },
		{ 0.6f, 0.15f },	// point
		{ 0.6f, 0.15f },
		{ 0.75f, 0.0f },
		{ 0.75f, 0.0f },	// point
		{ 0.75f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f }		// point
	};

	boundary_shape_C = {
		{ 0.0f, 0.0f },		// point
		{ 0.0f, 0.0f },
		{ 0.3f, 0.0f },
		{ 0.3f, 0.0f },		// point
		{ 0.3f, 0.0f },
		{ 0.33f, 0.01f },
		{ 0.33f, 0.04f },	// point
		{ 0.33f, 0.07f },
		{ 0.3f, 0.08f },
		{ 0.3f, 0.11f },	// point
		{ 0.3f, 0.14f },
		{ 0.33f, 0.17f },
		{ 0.39f, 0.17f },	// point
		{ 0.42f, 0.17f },
		{ 0.48f, 0.14f },
		{ 0.48f, 0.11f },	// point
		{ 0.48f, 0.08f },
		{ 0.45f, 0.07f },
		{ 0.45f, 0.04f },	// point
		{ 0.45f, 0.01f },
		{ 0.48f, 0.0f },
		{ 0.48f, 0.0f },	// point
		{ 0.48f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 0.0f }		// point
	};
}
