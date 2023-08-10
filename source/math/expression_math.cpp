#include "expression_math.h"


std::shared_ptr<Expression> operator+(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(left, '+', right);
}

std::shared_ptr<Expression> operator*(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
	return std::make_shared<CompoundExpression>(left, '*', right);
}