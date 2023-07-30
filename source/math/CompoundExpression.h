#pragma once
#include "Expression.h"
class CompoundExpression :
    public Expression
{
public:
	CompoundExpression(Expression* left, char op, Expression* right) :
		left(left),
		op(op),
		right(right)
	{}
	~CompoundExpression();

	float substitute(std::unordered_map<std::string, float> map);

private:
	Expression* left;	// left of operator
	Expression* right;	// right of operator
	char op;	// operator
};

