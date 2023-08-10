#pragma once
#include "Expression.h"

class ExpressionFactory
{
public:
	// Valid strings consist of decimal numbers, variables, and the operators +, -, *, and /.
	// Tokens must be separated by a space.
	// Variables start with a letter and can consist of letters, underscores, and numbers.
	// Examples: "3.14159", "v1_x + v2_x", "1 / x * 2 + y", "7.5 / -2.5"
	// Order of operations: * and /, then + and -. Left-to-right.
	// Unsupported: unary negation of variables, e.g. "-x", juxtaposition, e.g. "5x", parentheses
	std::shared_ptr<Expression> parse(const std::string& s);
};

