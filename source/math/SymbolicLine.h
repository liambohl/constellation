#pragma once
#include "expression_math.h"
#include "SymbolicPoint.h"

class SymbolicLine {
public:
	// Construct a line from two points
	SymbolicLine(SymbolicPoint p1, SymbolicPoint p2) : p1(p1), p2(p2) {}

	// Construct a line from two points defined with numbers
	SymbolicLine(float p1x, float p1y, float p2x, float p2y) : p1(p1x, p1y), p2(p2x, p2y) {}

	// Construct a line from two points defined with expressions
	SymbolicLine(std::shared_ptr<Expression> p1x, std::shared_ptr<Expression> p1y, std::shared_ptr<Expression> p2x, std::shared_ptr<Expression> p2y) : p1(p1x, p1y), p2(p2x, p2y) {}

	// Construct a line from two points defined with string expressions
	SymbolicLine(std::string p1x, std::string p1y, std::string p2x, std::string p2y) : p1(p1x, p1y), p2(p2x, p2y) {}

	SymbolicPoint get_vector() { return SymbolicPoint(p2.x - p1.x, p2.y - p1.y); }
	SymbolicPoint p1;
	SymbolicPoint p2;
};