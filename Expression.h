#pragma once

#include <string>
#include <unordered_map>

using std::string;

class Expression
{
public:
	Expression(string s);
	~Expression();

	Expression operator+(const Expression& other);
	Expression operator*(const Expression& other);
	float substitute(std::unordered_map<string, float> map);

private:
	Expression* left;	// left of operator
	Expression* right;	// right of operator
	char op;	// operator
};

