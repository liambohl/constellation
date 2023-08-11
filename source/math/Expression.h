#pragma once

#include <iostream>
#include <string>
#include <unordered_map>


class Expression
{
public:
	virtual float substitute(const std::unordered_map<std::string, float>& map = {}) = 0;

	virtual std::ostream& insert(std::ostream& os) const = 0;

protected:
	Expression() {}

	friend std::ostream& operator<<(std::ostream& os, const Expression* expression);
};
