#pragma once

#include "Canvas.h"

namespace Constellation {

	class Element
	{
	public:
		ULONG id;

		virtual void draw(Canvas& canvas) = 0;

		// Look for an element among this element and its children, if any.
		virtual Element* get_element(ULONG id);

	protected:
		Element(ULONG id) : id(id) {}
	};

}
