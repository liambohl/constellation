#include "ExpressionFactory.h"
#include "CompoundExpression.h"
#include "Value.h"
#include "Variable.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))

std::shared_ptr<Expression> ExpressionFactory::parse(const std::string& s) {
	size_t last_op;	// Index of the last operator at a given level of precedence

	// Scan for + or -
	size_t last_add = s.rfind(" + ");
	size_t last_subtract = s.rfind(" - ");
	if (last_add != std::string::npos && last_subtract != std::string::npos)
		last_op = max(last_add, last_subtract);
	else if (last_add != std::string::npos)
		last_op = last_add;
	else if (last_subtract != std::string::npos)
		last_op = last_subtract;
	else
		last_op = std::string::npos;

	if (last_op != std::string::npos) {
		std::shared_ptr<Expression> left = parse(s.substr(0, last_op));
		std::shared_ptr<Expression> right = parse(s.substr(last_op + 3));
		return std::make_shared<CompoundExpression>(left, s[last_op + 1], right);
	}

	// Scan for * or /
	size_t last_multiply = s.rfind(" * ");
	size_t last_divide = s.rfind(" / ");
	if (last_multiply != std::string::npos && last_divide != std::string::npos)
		last_op = max(last_multiply, last_divide);
	else if (last_multiply != std::string::npos)
		last_op = last_multiply;
	else if (last_divide != std::string::npos)
		last_op = last_divide;
	else
		last_op = std::string::npos;

	if (last_op != std::string::npos) {
		std::shared_ptr<Expression> left = parse(s.substr(0, last_op));
		std::shared_ptr<Expression> right = parse(s.substr(last_op + 3));
		return std::make_shared<CompoundExpression>(left, s[last_op + 1], right);
	}

	// Try to make a variable
	if (isalpha(s[0]))
		return std::make_shared<Variable>(s);

	// Try to make a value
	return std::make_shared<Value>(std::stof(s));
}