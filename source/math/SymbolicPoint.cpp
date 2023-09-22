#include "SymbolicPoint.h"

#include "ExpressionFactory.h"


SymbolicPoint::SymbolicPoint(std::string x, std::string y) {
	ExpressionFactory factory;
	this->x = factory.parse(x);
	this->y = factory.parse(y);
}
