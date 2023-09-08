#include "WallpaperGroup.h"

#include <cmath>
#include <numbers>

#define root3over2 0.8660254038

WallpaperGroup::WallpaperGroup(
	std::string name,
	shape cell_shape,
	std::vector<SymbolicMatrix> cell,
	std::vector<SymbolicPoint> rotation_centers,
	DomainBoundaries domain_boundaries,
	std::shared_ptr<SymmetryGroup> old
):
	SymmetryGroup(name), cell_shape(cell_shape), cell(cell), rotation_centers(rotation_centers), domain_boundaries(domain_boundaries)
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

void translate_matrix_in_world_space(Gdiplus::Matrix* matrix, float delta_x, float delta_y) {
	Gdiplus::Matrix translationMatrix;
	translationMatrix.Translate(delta_x, delta_y);

	matrix->Multiply(&translationMatrix, Gdiplus::MatrixOrderAppend);
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
		v2_direction = (float)(std::atan2(v1_y, v1_x) + std::numbers::pi);
		break;
	case RHOMBUS:
		// make v2 equal in length to v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		break;
	case HEXAGON:
		// make v2 equal in length and 60 degrees counter-clockwise from v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		v2_direction = (float)(std::atan2(v1_y, v1_x) + 2 / 3 * std::numbers::pi);
		break;
	case SQUARE:
		// make v2 equal in length and 90 degrees counter-clockwise from v1
		v2_length = std::sqrt(v1_x * v1_x + v1_y * v1_y);
		v2_direction = (float)(std::atan2(v1_y, v1_x) + std::numbers::pi);
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
		// make v1 perpendicular to v2
		v1_direction = (float)(std::atan2(v2_y, v2_x) + std::numbers::pi);
		break;
	case RHOMBUS:
		// make v1 equal in length to v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		break;
	case HEXAGON:
		// make v1 equal in length and 60 degrees counter-clockwise from v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		v1_direction = (float)(std::atan2(v2_y, v2_x) + 2 / 3 * std::numbers::pi);
		break;
	case SQUARE:
		// make v1 equal in length and 90 degrees counter-clockwise from v2
		v1_length = std::sqrt(v2_x * v2_x + v2_y * v2_y);
		v1_direction = (float)(std::atan2(v2_y, v2_x) + std::numbers::pi);
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

	// draw central cell
	Gdiplus::PointF vertices[6];
	int count;
	if (cell_shape == HEXAGON) {
		count = 6;
		// Clockwise starting from bottom left
		vertices[0] = { (-1 * v1_x - 1 * v2_x) / 3, (-1 * v1_y - 1 * v2_y) / 3 };
		vertices[1] = { (-2 * v1_x + 1 * v2_x) / 3, (-2 * v1_y + 1 * v2_y) / 3 };
		vertices[2] = { (-1 * v1_x + 2 * v2_x) / 3, (-1 * v1_y + 2 * v2_y) / 3 };
		vertices[3] = { ( 1 * v1_x + 1 * v2_x) / 3, ( 1 * v1_y + 1 * v2_y) / 3 };
		vertices[4] = { ( 2 * v1_x - 1 * v2_x) / 3, ( 2 * v1_y - 1 * v2_y) / 3 };
		vertices[5] = { ( 1 * v1_x - 2 * v2_x) / 3, ( 1 * v1_y - 2 * v2_y) / 3 };
	}
	else {
		count = 4;
		// Clockwise starting from bottom left
		vertices[0] = { (-v1_x - v2_x) / 2, (-v1_y - v2_y) / 2 };
		vertices[1] = { (-v1_x + v2_x) / 2, (-v1_y + v2_y) / 2 };
		vertices[2] = { ( v1_x + v2_x) / 2, ( v1_y + v2_y) / 2 };
		vertices[3] = { ( v1_x - v2_x) / 2, ( v1_y - v2_y) / 2 };
	}
	graphics->FillPolygon(defaults.symmetry_cell_brush, vertices, count);

	// draw domain boundaries
	for (auto& boundary : domain_boundaries.type_A)
		draw_boundary(graphics, defaults, boundary, variables, defaults.boundary_shape_A);

	for (auto& boundary : domain_boundaries.type_B)
		draw_boundary(graphics, defaults, boundary, variables, defaults.boundary_shape_B);

	for (auto& boundary : domain_boundaries.type_C)
		draw_boundary(graphics, defaults, boundary, variables, defaults.boundary_shape_C);
	
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

void WallpaperGroup::draw_boundary(
	Gdiplus::Graphics* graphics,
	Defaults& defaults,
	const SymbolicLine& boundary,
	const std::unordered_map<std::string, float>& variables,
	std::vector<Gdiplus::PointF> shape
) {
	// Get actual endpoints in world space
	float p1_x = boundary.p1.x->evaluate(variables);
	float p1_y = boundary.p1.y->evaluate(variables);
	float p2_x = boundary.p2.x->evaluate(variables);
	float p2_y = boundary.p2.y->evaluate(variables);

	// transformation from shape space to world space
	// This transform maps (0, 0) to p1 and (1, 0) to p2 with a rotation, scale, and translation
	Gdiplus::Matrix transform(p2_x - p1_x, p2_y - p1_y, p1_y - p2_y, p2_x - p1_x, p1_x, p1_y);

	// Actual bezier control points in world space
	std::vector<Gdiplus::PointF> control_points = shape;
	transform.TransformPoints(control_points.data(), (int)control_points.size());

	graphics->DrawBeziers(defaults.domain_boundary_pen, control_points.data(), (int)control_points.size());
}
