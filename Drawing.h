#pragma once
#include <vector>

#include "Canvas.h"
#include "Element.h"
#include "Group.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Constellation {

	// Represents an entire drawing with elements (shapes, curves, etc.) and a pattern
	class Drawing
	{
	public:
		Drawing();
		Drawing(json drawing_json);

		void draw(Canvas& canvas);

		json to_json();

		// Pattern pattern;
		Group elements;

	private:
		Gdiplus::Color background;				// "transparent" color shown where there are no elements
	};

}
