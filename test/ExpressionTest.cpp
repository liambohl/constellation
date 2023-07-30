#include <assert.h>

#include "Expression.h"

void test_expressions() {
	Expression* e = Expression::parse("v1");
	assert(e->substitute({ {"v1", 4.5f} }) == 4.5);
	e = Expression::parse("2 * 3 + 500 / 100");
	assert(e->substitute({ {"v1", 4.5f} }) == 11);
	e = Expression::parse("2 * v1 + 500 / v1");
	assert(e->substitute({ {"v1", 20} }) == 65);
	e = Expression::parse("2 * v1_x - 0.5 * v2_x");
	assert(e->substitute({ {"v1_x", 100}, {"v2_x", 3} }) == 198.5);
	e = Expression::parse("-8 * -9 / -6.0");
	assert(e->substitute({}) == -12.0);
}