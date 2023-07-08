#pragma once
#include "Drawing.h"

namespace Constellation {

	class Action
	{
	public:
		virtual void apply(Drawing& drawing) = 0;
		virtual void undo(Drawing& drawing) = 0;
	};

}
