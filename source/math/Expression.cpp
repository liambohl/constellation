#include "CompoundExpression.h"
#include "Expression.h"

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

