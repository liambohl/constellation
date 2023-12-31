#pragma once

#include "core/framework.h"

// Get a stream to the contents of the specified resource
Gdiplus::Bitmap* image_from_resource(HINSTANCE hInstance, int resourceID, const wchar_t* resourceType);
