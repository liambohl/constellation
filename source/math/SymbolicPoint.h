#pragma once

#include "Expression.h"
#include "Value.h"


class SymbolicPoint
{
public:
	// Origin
	SymbolicPoint() : SymbolicPoint(0, 0) {}

	// Construct a point from two numbers
	SymbolicPoint(float x, float y) : x(std::make_shared<Value>(x)), y(std::make_shared<Value>(y)) {}

	// Construct a point from two expressions
	SymbolicPoint(std::shared_ptr<Expression> x, std::shared_ptr<Expression> y) : x(x), y(y) {}

	// Construct a point by parsing two strings as expressions
	SymbolicPoint(std::string x, std::string y);

	std::shared_ptr<Expression> x;
	std::shared_ptr<Expression> y;
};

