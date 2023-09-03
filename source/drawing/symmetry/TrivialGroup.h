#pragma once
#include "SymmetryGroup.h"


class TrivialGroup :
    public SymmetryGroup
{
public:
	TrivialGroup() : SymmetryGroup("trivial") {}

	// Get the set of affine transforms necessary to tessalate a region.
	// In this case, just the identity transformation.
	std::vector<std::shared_ptr<Gdiplus::Matrix>> get_transforms() override {
		return { std::make_shared<Gdiplus::Matrix>() };
	}

	json to_json() override { return { { "type", "TrivialGroup" } }; }
};
