#pragma once

#include "Canvas.h"

namespace Constellation {

	class Element
	{
	public:
		virtual void draw(Canvas& canvas) = 0;
	};

}
