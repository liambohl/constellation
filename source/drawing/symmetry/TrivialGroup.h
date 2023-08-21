#pragma once
#include "SymmetryGroup.h"


class TrivialGroup :
    public SymmetryGroup
{
public:
	// Get the set of affine transforms necessary to tessalate a region.
	// In this case, just the identity transformation.
	std::vector<Gdiplus::Matrix*> tessalate() override { return { new Gdiplus::Matrix() }; }
};
