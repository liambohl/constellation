#include "CompoundExpression.h"
#include "expression_math.h"
#include "Value.h"


// Addition
std::shared_ptr<Expression> operator+(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(left, '+', right);
}

std::shared_ptr<Expression> operator+(float left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(std::make_shared<Value>(left), '+', right);
}

std::shared_ptr<Expression> operator+(std::shared_ptr<Expression> left, float right) {
	return std::make_shared<CompoundExpression>(left, '+', std::make_shared<Value>(right));
}

// Subtraction
std::shared_ptr<Expression> operator-(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(left, '-', right);
}

std::shared_ptr<Expression> operator-(float left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(std::make_shared<Value>(left), '-', right);
}

std::shared_ptr<Expression> operator-(std::shared_ptr<Expression> left, float right) {
	return std::make_shared<CompoundExpression>(left, '-', std::make_shared<Value>(right));
}

// Multiplication
std::shared_ptr<Expression> operator*(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(left, '*', right);
}

std::shared_ptr<Expression> operator*(float left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(std::make_shared<Value>(left), '*', right);
}

std::shared_ptr<Expression> operator*(std::shared_ptr<Expression> left, float right) {
	return std::make_shared<CompoundExpression>(left, '*', std::make_shared<Value>(right));
}

// Division
std::shared_ptr<Expression> operator/(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(left, '/', right);
}

std::shared_ptr<Expression> operator/(float left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(std::make_shared<Value>(left), '/', right);
}

std::shared_ptr<Expression> operator/(std::shared_ptr<Expression> left, float right) {
	return std::make_shared<CompoundExpression>(left, '/', std::make_shared<Value>(right));
}

// Negation
std::shared_ptr<Expression> operator-(std::shared_ptr<Expression> e) {
	return std::make_shared<CompoundExpression>(std::make_shared<Value>(0.0f), '-', e);
}
