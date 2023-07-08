#include "Drawing.h"

namespace Constellation {

	Drawing::~Drawing() {
		for (int i = 0; i < elements.size(); ++i)
		{
			delete elements[i];
		}
	}

	// Draw this drawing to the window
	void Drawing::draw(Canvas& canvas) {
		canvas.graphics->Clear(defaults.background);

		for (auto element : elements)
		{
			element->draw(canvas);
		}
	}

	void Drawing::add_element(Element* element) {
		elements.push_back(element);
	}


}
