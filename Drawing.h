#pragma once
#include <vector>

#include "Canvas.h"
#include "Defaults.h"
#include "Element.h"
#include "Group.h"

namespace Constellation {

	// Represents an entire drawing with elements (shapes, curves, etc.) and a pattern
	class Drawing
	{
	public:
		Drawing();

		ULONG new_id() { return next_id++; }

		void draw(Canvas& canvas);
		Element* get_element(ULONG id) { return elements.get_element(id); }

		Defaults defaults;

		// Pattern pattern;
		Group elements;

	private:
		ULONG next_id = 0;	// unique IDs for elements
	};

}
