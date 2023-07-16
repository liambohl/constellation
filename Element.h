#pragma once

#include "Canvas.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Constellation {

	class Element
	{
	public:
		ULONG id;

		virtual void draw(Canvas& canvas) = 0;
		
		virtual json to_json() = 0;

		// Look for an element among this element and its children, if any.
		virtual std::shared_ptr<Element> get_element(ULONG id);

	protected:
		Element(ULONG id) : id(id) {}
		Element(json element_json);
	};

}
