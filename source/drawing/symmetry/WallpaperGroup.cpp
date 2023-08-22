#include "WallpaperGroup.h"

#include <cmath>
#include <numbers>


std::vector<Gdiplus::Matrix*> WallpaperGroup::tessalate() {
	std::vector<Gdiplus::Matrix*> transforms;

	// Evaluate the cell with the current v1 and v2
	std::unordered_map<std::string, float> vectors = { {"v1_x", v1_x}, {"v1_y", v1_y}, {"v2_x", v2_x}, {"v2_y", v2_y} };
	std::vector<Gdiplus::Matrix*> cell_evaluated;
	for (auto& symbolic_matrix : cell)
		cell_evaluated.push_back(symbolic_matrix.evaluate(vectors).Clone());

	// For each copy of the cell
	for (int v1_factor = -extent; v1_factor <= extent; ++v1_factor) {
		for (int v2_factor = -extent; v2_factor <= extent; ++v2_factor) {
			// linear combination of v1 and v2
			float offset_x = v1_factor * v1_x + v2_factor * v2_x;
			float offset_y = v1_factor * v1_y + v2_factor * v2_y;

			// For each copy of the fundamental domain in a cell
			for (Gdiplus::Matrix* matrix : cell_evaluated) {
				// Translate
				Gdiplus::Matrix* translated = matrix->Clone();
				translated->Translate(offset_x, offset_y);
				transforms.push_back(translated);
			}
		}
	}

	return transforms;
}

void WallpaperGroup::set_v1(float x, float y) {
	v1_x = x;
	v1_y = y;

	float v2_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
	float v2_direction = std::atan2(v2_y, v2_x);

	switch (cell_shape) {
	case PARALLELOGRAM:
		break;
	case RECTANGLE:
		// make v2 perpendicular to v1
		v2_direction = std::atan2(v1_y, v1_x) + std::numbers::pi;
		break;
	case RHOMBUS:
		// make v2 equal in length to v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		break;
	case DIAMOND:
		// make v2 equal in length and 60 degrees counter-clockwise from v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		v2_direction = std::atan2(v1_y, v1_x) + 2 / 3 * std::numbers::pi;
		break;
	case SQUARE:
		// make v2 equal in length and 90 degrees counter-clockwise from v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		v2_direction = std::atan2(v1_y, v1_x) + std::numbers::pi;
		break;
	}

	v2_x = v2_length * std::cos(v2_direction);
	v2_y = v2_length * std::sin(v2_direction);
}

void WallpaperGroup::set_v2(float x, float y) {
	v2_x = x;
	v2_y = y;

	float v1_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
	float v1_direction = std::atan2(v1_y, v1_x);

	switch (cell_shape) {
	case PARALLELOGRAM:
		break;
	case RECTANGLE:
		// make v1 perpendicular to v2
		v1_direction = std::atan2(v2_y, v2_x) + std::numbers::pi;
		break;
	case RHOMBUS:
		// make v1 equal in length to v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		break;
	case DIAMOND:
		// make v1 equal in length and 60 degrees counter-clockwise from v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		v1_direction = std::atan2(v2_y, v2_x) + 2 / 3 * std::numbers::pi;
		break;
	case SQUARE:
		// make v1 equal in length and 90 degrees counter-clockwise from v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		v1_direction = std::atan2(v2_y, v2_x) + std::numbers::pi;
		break;
	}

	v1_x = v1_length * std::cos(v1_direction);
	v1_y = v1_length * std::sin(v1_direction);
}
