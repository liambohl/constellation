#include "WallpaperGroup.h"

#include <cmath>
#include <numbers>


#define root3over2 0.8660254038

WallpaperGroup::WallpaperGroup(
	std::string name,
	shape cell_shape,
	std::vector<SymbolicMatrix> cell,
	std::vector<SymbolicPoint> drawing_area,
	std::vector<SymbolicPoint> rotation_centers,
	DomainBoundaries domain_boundaries,
	std::shared_ptr<SymmetryGroup> old
):
	SymmetryGroup(name), cell_shape(cell_shape), cell(cell), drawing_area(drawing_area), rotation_centers(rotation_centers), domain_boundaries(domain_boundaries)
{
	v1_x = 100.0f;
	v1_y = 0.0f;
	switch (cell_shape) {
	case PARALLELOGRAM:
		v2_x = 30.0f;		// Arbitrarily make the parallelogram half as tall as it is wide and skewed right like /
		v2_y = -50.0f;
		break;
	case RECTANGLE:
		v2_x = 0.0f;		// Arbitrarily make the rectangle half as tall as it is wide.
		v2_y = -50.0f;
		break;
	case RHOMBUS:
		v2_x = 70.7107f;	// Arbitrarily make the rhombus a lozenge: a rhombus with angles of 45 and 135 degrees
		v2_y = -70.7107f;
	case HEXAGON:
		v2_x = 50.0f;
		v2_y = -86.6025f;
		break;
	case SQUARE:
		v2_x = 0.0f;
		v2_y = -100.0f;
		break;
	}

	// Try to match vectors
	if (std::shared_ptr<WallpaperGroup> old_wallpaper = dynamic_pointer_cast<WallpaperGroup>(old)) {
		set_v2(old_wallpaper->v2_x, old_wallpaper->v2_y);
		set_v1(old_wallpaper->v1_x, old_wallpaper->v1_y);
	}

	update_transforms();
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
		// make v2 90 degrees counter-clockwise from v1
		v2_direction = (float)(std::atan2(v1_y, v1_x) - std::numbers::pi / 2);
		break;
	case RHOMBUS:
		// make v2 equal in length to v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		break;
	case HEXAGON:
		// make v2 equal in length and 60 degrees counter-clockwise from v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		v2_direction = (float)(std::atan2(v1_y, v1_x) - std::numbers::pi / 3);
		break;
	case SQUARE:
		// make v2 equal in length and 90 degrees counter-clockwise from v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		v2_direction = (float)(std::atan2(v1_y, v1_x) - std::numbers::pi / 2);
		break;
	}

	v2_x = v2_length * std::cos(v2_direction);
	v2_y = v2_length * std::sin(v2_direction);

	update_transforms();
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
		// make v1 90 degrees clockwise from v2
		v1_direction = (float)(std::atan2(v2_y, v2_x) + std::numbers::pi / 2);
		break;
	case RHOMBUS:
		// make v1 equal in length to v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		break;
	case HEXAGON:
		// make v1 equal in length and 60 degrees clockwise from v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		v1_direction = (float)(std::atan2(v2_y, v2_x) + std::numbers::pi / 3);
		break;
	case SQUARE:
		// make v1 equal in length and 90 degrees clockwise from v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		v1_direction = (float)(std::atan2(v2_y, v2_x) + std::numbers::pi / 2);
		break;
	}

	v1_x = v1_length * std::cos(v1_direction);
	v1_y = v1_length * std::sin(v1_direction);

	update_transforms();
}

void WallpaperGroup::set_extent(int extent) {
	this->extent = extent;
	update_transforms();
}

void WallpaperGroup::draw(Gdiplus::Graphics* graphics, Defaults& defaults, float scale) {
	std::unordered_map<std::string, float> variables {
		{ "v1_x", v1_x },
		{ "v1_y", v1_y },
		{ "v2_x", v2_x },
		{ "v2_y", v2_y }
	};

	// draw drawing_area
	graphics->FillPolygon(defaults.drawing_area_brush, drawing_area_evaluated.data(), (int)drawing_area_evaluated.size());

	// draw domain boundaries
	domain_boundaries.draw(graphics, defaults);
	
	// draw rotation centers
	float radius = defaults.rotation_center_radius / scale;
	for (auto& point : rotation_centers) {
		float x = point.x->evaluate(variables);
		float y = point.y->evaluate(variables);
		graphics->FillEllipse(defaults.rotation_center_brush, x - radius, y - radius, 2 * radius, 2 * radius);
	}
}

json WallpaperGroup::to_json() {
	return {
		{"type", "WallpaperGroup"},
		{"name", name},
		{"v1_x", v1_x},
		{"v1_y", v1_y},
		{"v2_x", v2_x},
		{"v2_y", v2_y},
		{"extent", extent}
	};
}

void WallpaperGroup::update_transforms() {
	transforms.clear();

	// Evaluate the cell with the current v1 and v2
	std::unordered_map<std::string, float> vectors = { {"v1_x", v1_x}, {"v1_y", v1_y}, {"v2_x", v2_x}, {"v2_y", v2_y} };
	std::vector<std::shared_ptr<Gdiplus::Matrix>> cell_evaluated;
	for (auto& symbolic_matrix : cell)
		cell_evaluated.push_back(std::shared_ptr<Gdiplus::Matrix>(symbolic_matrix.evaluate(vectors).Clone()));

	// For each copy of the cell
	for (int v1_factor = -extent; v1_factor <= extent; ++v1_factor) {
		for (int v2_factor = -extent; v2_factor <= extent; ++v2_factor) {
			// The full extent of a hexagonal tessalation should be a big hexagon, not a bit rhombus.
			if (cell_shape == HEXAGON && abs(v1_factor + v2_factor) > extent)
				continue;

			// linear combination of v1 and v2
			float offset_x = v1_factor * v1_x + v2_factor * v2_x;
			float offset_y = v1_factor * v1_y + v2_factor * v2_y;

			// For each copy of the fundamental domain in a cell
			for (auto matrix : cell_evaluated) {
				// Translate
				auto translated = std::shared_ptr<Gdiplus::Matrix>(matrix->Clone());
				translated->Translate(offset_x, offset_y);
				transforms.push_back(translated);
			}
		}
	}

	// Update drawing area
	drawing_area_evaluated.clear();
	for (auto& symbolic_point : drawing_area) {
		drawing_area_evaluated.push_back({ symbolic_point.x->evaluate(vectors), symbolic_point.y->evaluate(vectors) });
	}

	// Update domain boundaries
	domain_boundaries.update(vectors);
}
