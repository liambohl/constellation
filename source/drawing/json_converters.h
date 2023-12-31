#pragma once

#include "core/framework.h"
#include "json.hpp"

using json = nlohmann::json;


json color_to_json(Gdiplus::Color color);
Gdiplus::Color color_from_json(json color_json);

json pen_to_json(Gdiplus::Pen* pen);
Gdiplus::Pen* pen_from_json(json pen_json);

json brush_to_json(Gdiplus::Brush* brush);
Gdiplus::Brush* brush_from_json(json brush_json);

json point_to_json(Gdiplus::PointF point);
Gdiplus::PointF point_from_json(json point_json);

json path_to_json(Gdiplus::GraphicsPath* path);
Gdiplus::GraphicsPath* path_from_json(json path_json);
