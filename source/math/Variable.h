#pragma once
#include "Expression.h"

class Variable :
    public Expression
{
public:
    Variable(std::string name) : name(name) {}

    float evaluate(const std::unordered_map<std::string, float>& map) { return map.at(name); }
    
    std::ostream& insert(std::ostream& os) const { return os << name; }

private:
    std::string name;
};
