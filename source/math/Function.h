#pragma once

#include "Expression.h"


class Function :
    public Expression
{
public:
	Function(std::string function, std::shared_ptr<Expression> argument) : function(function), argument(argument) {}

	virtual float evaluate(const std::unordered_map<std::string, float>& map);

	virtual std::ostream& insert(std::ostream& os) const;

private:
	std::string function;
	std::shared_ptr<Expression> argument;
};

