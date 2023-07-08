#pragma once
#include <vector>

#include "Canvas.h"
#include "Defaults.h"
#include "Element.h"

namespace Constellation {

	// Represents an entire drawing with elements (shapes, curves, etc.) and a pattern
	class Drawing
	{
		//private:
	public:
		// Pattern pattern;
		std::vector<Element*> elements;
		Defaults defaults;

		~Drawing();

	public:
		void draw(Canvas& canvas);
		void add_element(Element* element);
	};

}
