#pragma once

#include "Expression.h"


class ExpressionFactory
{
public:
	// Valid strings consist of decimal numbers, variables, functions, and the operators +, -, *, and /.
	// The supported functions are sin, cos, and sqrt. Trig is performed in degrees.
	// Operators should be preceded and followed by a space.
	// Variables start with a letter and can consist of letters, underscores, and numbers.
	// Examples: 
	//     3.14159
	//     v1_x + v2_x
	//     1 / x * 2 + y
	//     7.5 / -2.5
	//     (1 + x) * (cos(y))
	//     -sin(theta) * cos(theta)
	// Order of operations: parentheses, functions, negation, * and /, + and -. Left-to-right.
	// Unsupported: juxtaposition, e.g. "5x
	std::shared_ptr<Expression> parse(std::string s);

private:
	size_t find_last_outside_parentheses(const std::string& s, const std::string& sub_a, const std::string& sub_b);
};

