#pragma once

#include <string>
#include <unordered_map>


class Expression
{
public:
	// Valid strings consist of decimal numbers, variables, and the operators +, -, *, and /.
	// Tokens must be separated by a space.
	// Variables start with a letter and can consist of letters, underscores, and numbers.
	// Examples: "3.14159", "v1_x + v2_x", "1 / x * 2 + y", "7.5 / -2.5"
	// Order of operations: * and /, then + and -. Left-to-right.
	// Unsupported: unary negation, e.g. "-x", juxtaposition, e.g. "5x", parentheses
	static Expression* parse(std::string s);

	Expression* operator+(Expression* other);
	Expression* operator-(Expression* other);
	Expression* operator*(Expression* other);
	Expression* operator/(Expression* other);

	virtual float substitute(std::unordered_map<std::string, float> map) = 0;

protected:
	Expression() {}
};

