#pragma once
#include <vector>

#include "Canvas.h"
#include "Defaults.h"
#include "Element.h"
#include "Group.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Constellation {

	// Represents an entire drawing with elements (shapes, curves, etc.) and a pattern
	class Drawing
	{
	public:
		Drawing() {}
		Drawing(json drawing_json);

		void draw(Canvas& canvas);

		json to_json();

		Defaults defaults;

		// Pattern pattern;
		Group elements;

	private:
		ULONG next_id = 0;	// unique IDs for elements
	};

}
