#pragma once
#include <memory>
#include <string>
#include <vector>

#include "core/Defaults.h"
#include "core/framework.h"
#include "json.hpp"

using json = nlohmann::json;

class SymmetryGroup
{
public:
	// Get the set of affine transforms necessary to tessalate a region.
	// Each matrix is an affine transformation that should be applied to each element of the drawing.
	virtual std::vector<std::shared_ptr<Gdiplus::Matrix>> get_transforms() = 0;

	// Draw guides for this symmetry group.
	// Scale is the current canvas zoom scale. This parameter allows us to draw guides with sizes that do not respond to zoom.
	virtual void draw(Gdiplus::Graphics* graphics, Defaults& defaults, float scale) = 0;

	virtual json to_json() = 0;

	std::string name;

protected:
	SymmetryGroup(std::string name) : name(name) {}
};
