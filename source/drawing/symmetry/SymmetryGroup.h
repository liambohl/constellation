#pragma once
#include <vector>

#include "core/framework.h"


class SymmetryGroup
{
public:
	// Get the set of affine transforms necessary to tessalate a region.
	// Each matrix is an affine transformation that should be applied to each element of the drawing.
	virtual std::vector<Gdiplus::Matrix*> tessalate() = 0;
};
