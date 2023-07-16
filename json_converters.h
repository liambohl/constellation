#pragma once
#include "framework.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Constellation {

	json color_to_json(Gdiplus::Color color);
	Gdiplus::Color color_from_json(json color_json);
	json pen_to_json(Gdiplus::Pen* pen);
	Gdiplus::Pen* pen_from_json(json pen_json);
	json brush_to_json(Gdiplus::Brush* brush);
	Gdiplus::Brush* brush_from_json(json brush_json);
	json point_to_json(Gdiplus::Point point);
	Gdiplus::Point point_from_json(json point_json);

}
