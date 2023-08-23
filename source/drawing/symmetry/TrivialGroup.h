#pragma once
#include "SymmetryGroup.h"


class TrivialGroup :
    public SymmetryGroup
{
public:
	TrivialGroup() : SymmetryGroup("trivial") {}

	// Get the set of affine transforms necessary to tessalate a region.
	// In this case, just the identity transformation.
	std::vector<Gdiplus::Matrix*> get_transforms() override { return { new Gdiplus::Matrix() }; }
};
