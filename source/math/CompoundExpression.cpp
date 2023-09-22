#include "CompoundExpression.h"


float CompoundExpression::evaluate(const std::unordered_map<std::string, float>& map) {
	float sub_left = left->evaluate(map);
	float sub_right = right->evaluate(map);
	switch (op) {
	case '+':
		return sub_left + sub_right;
	case '-':
		return sub_left - sub_right;
	case '*':
		return sub_left * sub_right;
	case '/':
		return sub_left / sub_right;
	default:
		throw std::exception("Invalid operation");
	}
}

std::ostream& CompoundExpression::insert(std::ostream& os) const {
	left->insert(os);
	os << " " << op << " ";
	right->insert(os);
	return os;
}
