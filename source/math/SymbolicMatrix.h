#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Expression.h"
#include "SymbolicPoint.h"
#include "SymbolicLine.h"
#include "core/framework.h"


// This class represents an affine transformation in 2 dimensions, defined in terms of zero or more variables.
// Note that Gdiplus::Matrix uses the convention of writing points as row vectors
// and multiplying them by matrices on the right. This class follows the same convention.
class SymbolicMatrix {
public:
    // The identity transform
    SymbolicMatrix();

    // A matrix representing a translation by (dx, dy)
    static SymbolicMatrix translate(SymbolicPoint point);

    // A matrix representing a rotation of theta degrees counter-clockwise around a given point
    static SymbolicMatrix rotate(float theta, SymbolicPoint point = SymbolicPoint(0.0f, 0.0f));

    // A matrix representing a reflex
    static SymbolicMatrix reflect(SymbolicLine line);

    // Substitute each variable in the matrix with a number
    Gdiplus::Matrix evaluate(const std::unordered_map<std::string, float>& map = {});

private:
    // Just like subscripting the matrix itself, subscripting this class gives a reference to a row of the matrix. 
    std::shared_ptr<Expression> (&operator[](int index))[3] { return matrix[index]; }

    friend SymbolicMatrix operator*(SymbolicMatrix left, SymbolicMatrix right);
    
    friend std::ostream& operator<<(std::ostream& os, const SymbolicMatrix& transform);

    std::shared_ptr<Expression> matrix[3][3];
};