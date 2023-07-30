#pragma once
#include "Expression.h"

class Variable :
    public Expression
{
public:
    Variable(std::string name) : name(name) {}

    float substitute(std::unordered_map<std::string, float> map) { return map[name]; }

private:
    std::string name;
};
