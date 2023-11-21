#pragma once

#include "Expression.h"

#include <memory>


class CompoundExpression :
    public Expression
{
public:
    CompoundExpression(std::shared_ptr<Expression> left, char op, std::shared_ptr<Expression> right) :
        left(left),
        op(op),
        right(right)
    {}

    float evaluate(const std::unordered_map<std::string, float>& map);

    std::ostream& insert(std::ostream& os) const;

private:
    std::shared_ptr<Expression> left;   // left of operator
    std::shared_ptr<Expression> right;  // right of operator

    char op;    // operator
};

