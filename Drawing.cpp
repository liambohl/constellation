#include "Drawing.h"
#include "json_converters.h"
#include "Logger.h"

Drawing::Drawing() {
	background = Gdiplus::Color(255, 80, 80, 80);
}

Drawing::Drawing(json drawing_json) :
	background(color_from_json(drawing_json["background"])),
	elements(drawing_json["elements"])
{}

// Draw this drawing to the window
void Drawing::draw(Canvas& canvas) {
	canvas.graphics->Clear(background);

	elements.draw(canvas);
}

json Drawing::to_json() {
	return {
		{"background", color_to_json(background)},
		{"elements", elements.to_json()}
	};
}
