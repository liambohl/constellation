#include "ToolSelect.h"


Action* ToolSelect::handle_mouse_event(UINT message, float x_pos, float y_pos, int key_state, float scale) {
	return nullptr;
}

void ToolSelect::draw(
	Gdiplus::Graphics* graphics,
	Gdiplus::PointF cursor_pos,
	std::vector<std::shared_ptr<Gdiplus::Matrix>>& transforms,
	float scale
) {

}
