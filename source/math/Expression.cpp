#include "CompoundExpression.h"
#include "Expression.h"
#include "Value.h"
#include "Variable.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

Expression* Expression::parse(std::string s) {
	// Scan for + or -
	size_t op_index = min(s.find(" + "), s.find(" - "));
	if (op_index != std::string::npos) {
		Expression* left = parse(s.substr(0, op_index));
		Expression* right = parse(s.substr(op_index + 3));
		return new CompoundExpression(left, s[op_index + 1], right);
	}

	// Scan for * or /
	op_index = min(s.find(" * "), s.find(" / "));
	if (op_index != std::string::npos) {
		Expression* left = parse(s.substr(0, op_index));
		Expression* right = parse(s.substr(op_index + 3));
		return new CompoundExpression(left, s[op_index + 1], right);
	}

	// Try to make a variable
	if (isalpha(s[0]))
		return new Variable(s);

	// Try to make a value
	return new Value(std::stof(s));
}

Expression* Expression::operator+(Expression* other) {
	return new CompoundExpression(this, '+', other);
}

Expression* Expression::operator-(Expression* other) {
	return new CompoundExpression(this, '-', other);
}

Expression* Expression::operator*(Expression* other) {
	return new CompoundExpression(this, '*', other);
}

Expression* Expression::operator/(Expression* other) {
	return new CompoundExpression(this, '/', other);
}

