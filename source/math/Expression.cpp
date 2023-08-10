#include "Expression.h"


std::ostream& operator<<(std::ostream& os, const Expression* expression) {
	return expression->insert(os);
}
