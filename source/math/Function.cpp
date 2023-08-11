#include <cmath>
#include <numbers>

#include "Function.h"

float degrees_to_radians(float angle) {
	return (float)(angle * std::numbers::pi / 180);
}

float Function::evaluate(const std::unordered_map<std::string, float>& map) {
	float argument_value = argument->evaluate(map);

	if (function == "sin")
		return sin(degrees_to_radians(argument_value));
	if (function == "cos")
		return cos(degrees_to_radians(argument_value));
	if (function == "sqrt")
		return sqrt(argument_value);
	throw new std::exception("unknown function");
}

std::ostream& Function::insert(std::ostream& os) const {
	os << function << "(" << argument << ")";
	return os;
}
