#include "ExpressionFactory.h"
#include "CompoundExpression.h"
#include "Function.h"
#include "Value.h"
#include "Variable.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))

std::shared_ptr<Expression> ExpressionFactory::parse(const std::string& s) {
	size_t last_op;	// Index of the last operator at a given level of precedence

	// + or -
	last_op = find_last_outside_parentheses(s, " + ", " - ");
	if (last_op != std::string::npos) {
		std::shared_ptr<Expression> left = parse(s.substr(0, last_op));
		std::shared_ptr<Expression> right = parse(s.substr(last_op + 3));
		return std::make_shared<CompoundExpression>(left, s[last_op + 1], right);
	}

	// * or /
	last_op = find_last_outside_parentheses(s, " * ", " / ");
	if (last_op != std::string::npos) {
		std::shared_ptr<Expression> left = parse(s.substr(0, last_op));
		std::shared_ptr<Expression> right = parse(s.substr(last_op + 3));
		return std::make_shared<CompoundExpression>(left, s[last_op + 1], right);
	}

	// negation
	if (s[0] == '-') {
		// negative value
		if (isdigit(s[1]))
			return std::make_shared<Value>(std::stof(s));
		// negated expression
		else
			return std::make_shared<CompoundExpression>(std::make_shared<Value>(-1.0f), '*', parse(s.substr(1)));
	}

	// functions and parentheses
	if (s[s.length() - 1] == ')') {
		// functions
		if (isalpha(s[0])) {
			size_t left_paren = s.find('(');
			std::string function = s.substr(0, left_paren);
			std::string argument = s.substr(left_paren + 1, s.length() - left_paren - 2);
			return std::make_shared<Function>(function, parse(argument));
		}

		// parentheses
		return parse(s.substr(1, s.length() - 2));
	}

	// Try to make a variable
	if (isalpha(s[0]))
		return std::make_shared<Variable>(s);

	// Try to make a value
	return std::make_shared<Value>(std::stof(s));
}

// Assumption: the given substrings are the same length
size_t ExpressionFactory::find_last_outside_parentheses(const std::string& s, const std::string& sub_a, const std::string& sub_b) {
	int paren_level = 0;	// number of nested parenthese level we are currently inside
	size_t length = s.length();
	size_t sub_length = sub_a.length();

	// Look backward through the string for instances of substring outside parentheses
	for (size_t i = length - 1; i >= sub_length - 1; --i) {
		if (s[i] == ')')
			++paren_level;
		else if (s[i] == '(')
			--paren_level;
		else if (paren_level == 0) {
			std::string substring = s.substr(i - sub_length + 1, sub_length);
			if (substring == sub_a || substring == sub_b)
				return i - sub_length + 1;	// found it!
		}
	}

	return std::string::npos;
}
