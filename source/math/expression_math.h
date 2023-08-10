#pragma once
#include <memory>

#include "CompoundExpression.h"
#include "Expression.h"


std::shared_ptr<Expression> operator+(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);

std::shared_ptr<Expression> operator*(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
