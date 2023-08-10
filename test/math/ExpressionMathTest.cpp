#include <stdio.h>

#include "pch.h"

#include "math/expression_math.h"
#include "math/ExpressionFactory.h"

TEST(ExpressionMathTest, adds_and_multiplies_constants) {
	std::shared_ptr<Expression> a, b, c, d, result;
	ExpressionFactory factory;
	a = factory.parse("12");
	b = factory.parse("1000");
	c = factory.parse("5");
	d = factory.parse("1");
	result = a * b + c * d;
	EXPECT_EQ(result->substitute({}), 12005);
	EXPECT_NE(result->substitute({}), 1000);
}

TEST(ExpressionMathTest, adds_and_multiplies_variables) {
	std::shared_ptr<Expression> a, b, c, d, result;
	ExpressionFactory factory;
	a = factory.parse("12");
	b = factory.parse("x");
	c = factory.parse("5");
	d = factory.parse("y");
	result = a * b + c * d;
	EXPECT_EQ(result->substitute({ {"x", 1}, {"y", 1000} }), 5012);
}