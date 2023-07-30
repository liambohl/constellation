#pragma once

#include <string>
#include <unordered_map>

#include "framework.h"

using std::string;

// This class represents an affine transformation as a matrix whose elements may include variables
class SymbolicTransform {
public:
	// The identity transform
	SymbolicTransform();

	// Substitute each variable in the matrix with a number
	Gdiplus::Matrix substitute(std::unordered_map<string, float> map);

private:

	string matrix[3][3] = { {"1", "0", "0"}, {"0", "1,", "0"}, {"0", "0", "0"} };
};