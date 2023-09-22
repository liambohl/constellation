#include "SymbolicMatrix.h"

#include <cmath>
#include <iomanip>
#include <numbers>
#include <sstream>

#include "expression_math.h"
#include "ExpressionFactory.h"
#include "Value.h"


SymbolicMatrix::SymbolicMatrix() {
	// All zeroes
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			matrix[i][j] = std::make_shared<Value>(0.0f);

	// All ones on the main diagonal
	matrix[0][0] = std::make_shared<Value>(1.0f);
	matrix[1][1] = std::make_shared<Value>(1.0f);
	matrix[2][2] = std::make_shared<Value>(1.0f);
}

SymbolicMatrix SymbolicMatrix::translate(SymbolicPoint point) {
	SymbolicMatrix m;
	m[2][0] = point.x;
	m[2][1] = point.y;
	return m;
}

// ⎡ cos(θ)   -sin(θ)   -cos(θ) * x + x + sin(θ) * y ⎤
// ⎢ sin(θ)    cos(θ)   -sin(θ) * x - cos(θ) * y + y ⎥
// ⎣      0         0                              1 ⎦
SymbolicMatrix SymbolicMatrix::rotate(float theta, SymbolicPoint point) {
	theta *= (float)std::numbers::pi / 180.0f;	// Convert from degrees to radians
	auto sin_theta = std::make_shared<Value>((float)sin(theta));
	auto cos_theta = std::make_shared<Value>((float)cos(theta));
	auto x = point.x;
	auto y = point.y;
	
	SymbolicMatrix m;
	m[0][0] = cos_theta;
	m[0][1] = sin_theta;
	m[1][0] = -sin_theta;
	m[1][1] = cos_theta;
	m[2][0] = -cos_theta * x + x + sin_theta * y;
	m[2][1] = -sin_theta * x - cos_theta * y + y;
	return m;
}

//        2      ⎡ A²  AB  AC ⎤
// I - ------- * ⎢ AB  B²  BC ⎥
//     A² + B²   ⎣  0   0   0 ⎦
SymbolicMatrix SymbolicMatrix::reflect(SymbolicLine line) {
	auto x1 = line.p1.x;
	auto y1 = line.p1.y;
	auto x2 = line.p2.x;
	auto y2 = line.p2.y;

	// The line can be defined as Ax + By + C = 0
	auto A = y2 - y1;
	auto B = x1 - x2;
	auto C = x2 * y1 - x1 * y2;

	// See comment above function
	auto common_factor = -2.0f / (A*A + B*B);

	SymbolicMatrix m;
	m[0][0] = 1.0f + A * A * common_factor;
	m[0][1] = A * B * common_factor;
	m[1][0] = A * B * common_factor;
	m[1][1] = 1.0f + B * B * common_factor; 
	m[2][0] = A * C * common_factor;
	m[2][1] = B * C * common_factor;
	return m;
}

Gdiplus::Matrix SymbolicMatrix::evaluate(const std::unordered_map<std::string, float>& map) {
	float m11 = matrix[0][0]->evaluate(map);
	float m12 = matrix[0][1]->evaluate(map);
	float m21 = matrix[1][0]->evaluate(map);
	float m22 = matrix[1][1]->evaluate(map);
	float dx  = matrix[2][0]->evaluate(map);
	float dy  = matrix[2][1]->evaluate(map);
	return Gdiplus::Matrix(m11, m12, m21, m22, dx, dy);
}

SymbolicMatrix operator*(SymbolicMatrix left, SymbolicMatrix right) {
	SymbolicMatrix result;

	// For each element result[i][j] of the result,
	// Sum three products.
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			result[i][j] = left[i][0] * right[0][j] + left[i][1] * right[1][j] + left[i][2] * right[2][j];
		}
	}

	return result;
}

std::ostream& operator<<(std::ostream& os, const SymbolicMatrix& transform) {
	// First, use << to write each element to a string.
	std::string elements[3][3];
	size_t length;
	size_t max_length = 0;	// longest expression
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			std::stringstream sstream;
			sstream << (transform.matrix[i][j]);
			elements[i][j] = sstream.str();
			length = sstream.str().length();
			if (length > max_length)
				max_length = length;
		}
	}


	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			os << std::setw(max_length) << elements[i][j] << ",  ";
		}
		os << std::endl;
	}

	return os;
}
