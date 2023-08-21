#include "WallpaperGroup.h"

#include <algorithm>

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
