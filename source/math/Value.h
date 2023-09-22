#pragma once

#include "Expression.h"


class Value :
    public Expression
{
public:
    Value(float value) : value(value) {}

    float evaluate(const std::unordered_map<std::string, float>& map) { return value; }
    
    std::ostream& insert(std::ostream& os) const { return os << value; }

private:
    friend std::shared_ptr<Value> operator-(std::shared_ptr<Value> e);

    float value;
};
