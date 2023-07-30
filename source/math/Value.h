#pragma once
#include "Expression.h"

class Value :
    public Expression
{
public:
    Value(float value) : value(value) {}

    float substitute(std::unordered_map<std::string, float> map) { return value; }

private:
    float value;
};
