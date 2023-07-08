#include "Drawing.h"

namespace Constellation {

	Drawing::Drawing() : elements(new_id()) {}

	// Draw this drawing to the window
	void Drawing::draw(Canvas& canvas) {
		canvas.graphics->Clear(defaults.background);

		elements.draw(canvas);
	}

}
