#include "CompoundExpression.h"

CompoundExpression::~CompoundExpression() {
	delete left;
	delete right;
}

float CompoundExpression::substitute(std::unordered_map<std::string, float> map) {
	float sub_left = left->substitute(map);
	float sub_right = right->substitute(map);
	switch (op) {
	case '+':
		return sub_left + sub_right;
	case '-':
		return sub_left - sub_right;
	case '*':
		return sub_left * sub_right;
	case '/':
		return sub_left / sub_right;
	}
}
