#include "pch.h"

#include "math/ExpressionFactory.h"

TEST(ExpressionTest, handles_expressions_without_variables) {
	std::shared_ptr<Expression> e;
	ExpressionFactory factory;
	e = factory.parse("2 * 3 + 500 / 100");
	EXPECT_EQ(e->evaluate({ {"v1", 4.5f} }), 11.0f) << e;
	e = factory.parse("-8 * -9 / -6.0");
	EXPECT_EQ(e->evaluate(), -12.0f) << e;
}

TEST(ExpressionTest, handles_expressions_with_one_variable) {
	std::shared_ptr<Expression> e;
	ExpressionFactory factory;
	e = factory.parse("v1");
	EXPECT_EQ(e->evaluate({ {"v1", 4.5f} }), 4.5f) << e;
	e = factory.parse("2 * v1 + 500 / v1");
	EXPECT_EQ(e->evaluate({ {"v1", 20.0f} }), 65.0f) << e;
}

TEST(ExpressionTest, handles_expressions_with_multiple_variables) {
	std::shared_ptr<Expression> e;
	ExpressionFactory factory;
	e = factory.parse("2 * v1_x - 0.5 * v2_x");
	EXPECT_EQ(e->evaluate({ {"v1_x", 100.0f}, {"v2_x", 3.0f} }), 198.5f) << e;
	e = factory.parse("a + b + c / 2");
	EXPECT_EQ(e->evaluate({ {"a", 1000.0f}, {"b", 100.0f}, {"c", 20.0f} }), 1110.0f) << e;
	e = factory.parse("a - b + c / 2");
	EXPECT_EQ(e->evaluate({ {"a", 1000.0f}, {"b", 100.0f}, {"c", 20.0f} }), 910.0f) << e;
	e = factory.parse("0 - a - b - c / 2");
	EXPECT_EQ(e->evaluate({ {"a", 1000.0f}, {"b", 100.0f}, {"c", 20.0f} }), -1110.0f) << e;
}

TEST(ExpressionTest, handles_functions) {
	std::shared_ptr<Expression> e, f;
	ExpressionFactory factory;
	e = factory.parse("sin(45) * sqrt(2)");
	EXPECT_FLOAT_EQ(e->evaluate(), 1.0f) << e;
	e = factory.parse("(cos(0) + cos(0)) * cos(30)");
	f = factory.parse("sqrt(3)");
	EXPECT_FLOAT_EQ(e->evaluate(), f->evaluate()) << e << std::endl << f;
	e = factory.parse("sin(90 * cos(0))");
	EXPECT_FLOAT_EQ(e->evaluate(), 1.0f);
}