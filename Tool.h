#pragma once
#include "Canvas.h"
#include "Drawing.h"

namespace Constellation {
		
	class Tool
	{
	protected:
		Drawing& drawing;

		Tool(Drawing& drawing) : drawing(drawing) {}

	public:
		virtual void handleMouseEvent(UINT message, WPARAM wParam, LPARAM lParam) = 0;
		virtual void draw(Canvas& canvas) = 0;
	};

}
