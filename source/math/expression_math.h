#pragma once
#include <memory>

#include "Expression.h"

// Addition
std::shared_ptr<Expression> operator+(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
std::shared_ptr<Expression> operator+(float left, std::shared_ptr<Expression> right);
std::shared_ptr<Expression> operator+(std::shared_ptr<Expression> left, float right);

// Subtraction
std::shared_ptr<Expression> operator-(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
std::shared_ptr<Expression> operator-(float left, std::shared_ptr<Expression> right);
std::shared_ptr<Expression> operator-(std::shared_ptr<Expression> left, float right);

// Multiplication
std::shared_ptr<Expression> operator*(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
std::shared_ptr<Expression> operator*(float left, std::shared_ptr<Expression> right);
std::shared_ptr<Expression> operator*(std::shared_ptr<Expression> left, float right);

// Division
std::shared_ptr<Expression> operator/(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
std::shared_ptr<Expression> operator/(float left, std::shared_ptr<Expression> right);
std::shared_ptr<Expression> operator/(std::shared_ptr<Expression> left, float right);

// Negation
std::shared_ptr<Expression> operator-(std::shared_ptr<Expression> e);
