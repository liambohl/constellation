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
	EXPECT_EQ(result->evaluate(), 12005.0f);
	EXPECT_NE(result->evaluate(), 1000.0f);
}

TEST(ExpressionMathTest, adds_and_multiplies_variables) {
	std::shared_ptr<Expression> a, b, c, d, result;
	ExpressionFactory factory;
	a = factory.parse("12");
	b = factory.parse("x");
	c = factory.parse("5");
	d = factory.parse("y");
	result = a * b + c * d;
	EXPECT_EQ(result->evaluate({ {"x", 1.0f}, {"y", 1000.0f} }), 5012.0f);
}