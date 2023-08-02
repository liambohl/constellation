#pragma once

#include <string>
#include <unordered_map>


class Expression
{
public:
	Expression* operator+(Expression* other);
	Expression* operator-(Expression* other);
	Expression* operator*(Expression* other);
	Expression* operator/(Expression* other);

	virtual float substitute(std::unordered_map<std::string, float> map) = 0;

protected:
	Expression() {}
};

