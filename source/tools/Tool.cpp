#include "Tool.h"

void Tool::draw_handle(Gdiplus::Graphics* graphics, float x, float y, float scale, bool selected) {
	float radius = defaults.handle_radius / scale;

	if (selected)
		graphics->FillEllipse(defaults.selected_handle_brush, x - radius, y - radius, 2 * radius, 2 * radius);
	else
		graphics->FillEllipse(defaults.handle_brush, x - radius, y - radius, 2 * radius, 2 * radius);

	graphics->DrawEllipse(defaults.handle_pen, x - radius, y - radius, 2 * radius, 2 * radius);
}
