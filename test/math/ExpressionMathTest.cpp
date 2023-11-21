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

TEST(ExpressionMathTest, subtracts_and_divides_constants) {
    std::shared_ptr<Expression> a, b, c, d, result;
    ExpressionFactory factory;
    a = factory.parse("12");
    b = factory.parse("1000");
    c = factory.parse("5");
    d = factory.parse("1");
    result = (a + b) / (c - d);
    EXPECT_EQ(result->evaluate(), 253.0f);
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

TEST(ExpressionMathTest, subtracts_and_divides_variables) {
    std::shared_ptr<Expression> a, b, c, d, result;
    ExpressionFactory factory;
    a = factory.parse("12");
    b = factory.parse("x");
    c = factory.parse("5");
    d = factory.parse("y");
    result = (a + b) / (c - d);
    EXPECT_EQ(result->evaluate({ {"x", 1.0f}, {"y", 105.0f} }), -0.13f);
}


TEST(ExpressionMathTest, all_math_with_floats_with_expressions) {
    std::shared_ptr<Expression> a, b, c, d, result;
    ExpressionFactory factory;
    a = factory.parse("1");
    b = factory.parse("x");
    c = factory.parse("3");
    d = factory.parse("y");
    result = -(a * b) / (2 + c - d - 3);    // -(1 * 2) / (2 + 3 - 4 - 3)
    EXPECT_EQ(result->evaluate({ {"x", 2.0f}, {"y", 4.0f} }), 1.0f);
}
