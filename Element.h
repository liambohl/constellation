#pragma once

#include "framework.h"
#include "json.hpp"

using json = nlohmann::json;

class Element
{
public:
	virtual void draw(Gdiplus::Graphics* graphics) = 0;
	virtual void get_bounding_box(Gdiplus::RectF** bounding_box) = 0;

	virtual json to_json() = 0;

protected:
	Element() {}
};
