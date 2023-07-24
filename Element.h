#pragma once

#include "Canvas.h"
#include "json.hpp"

using json = nlohmann::json;

class Element
{
public:
	virtual void draw(Canvas& canvas) = 0;
		
	virtual json to_json() = 0;

protected:
	Element() {}
};
