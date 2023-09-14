#pragma once
#include <unordered_map>
#include <vector>

#include "core/Defaults.h"
#include "core/framework.h"
#include "math/SymbolicLine.h"

// This class bundles the edges of all fundamental domains in a wallpaper group's central cell.
// Edges that are congruent under one of the symmetry group's transformations should have the same type.
// Edges are drawn differently depending on their type
class DomainBoundaries
{
public:
	DomainBoundaries();

	void add_mirror_line  (SymbolicLine line) { mirror_lines .push_back(line); }
	void add_type_A       (SymbolicLine line) { type_A       .push_back(line); }
	void add_type_A_mirror(SymbolicLine line) { type_A_mirror.push_back(line); }
	void add_type_B       (SymbolicLine line) { type_B       .push_back(line); }
	void add_type_B_mirror(SymbolicLine line) { type_B_mirror.push_back(line); }
	void add_type_C       (SymbolicLine line) { type_C       .push_back(line); }
	void add_type_C_mirror(SymbolicLine line) { type_C_mirror.push_back(line); }

	void update(std::unordered_map<std::string, float>& variables);

	void draw(Gdiplus::Graphics* graphics, Defaults& defaults);

private:
	// Get bezier control points in world space for one domain boundary
	std::vector<Gdiplus::PointF> evaluate_one(
		SymbolicLine& endpoints,
		std::vector<Gdiplus::PointF>& shape,
		std::unordered_map<std::string, float>& variables,
		bool mirrored=false
	);

	// Each of these represents a Bezier curve from (0, 0) to (1, 0).
	// These shapes are constant
	std::vector<Gdiplus::PointF> mirror_line;			// straight line
	std::vector<Gdiplus::PointF> boundary_shape_A;		// bump out
	std::vector<Gdiplus::PointF> boundary_shape_B;		// zig zag out
	std::vector<Gdiplus::PointF> boundary_shape_C;		// puzzle piece key

	// Each SymbolicLine defines the endpoints of a domain boundary in terms of v1 and v2.
	// SymbolicLines are grouped based on how they should be drawn.
	std::vector<SymbolicLine> mirror_lines;
	std::vector<SymbolicLine> type_A;
	std::vector<SymbolicLine> type_A_mirror;
	std::vector<SymbolicLine> type_B;
	std::vector<SymbolicLine> type_B_mirror;
	std::vector<SymbolicLine> type_C;
	std::vector<SymbolicLine> type_C_mirror;

	// For each type of line, a collection of lines, each defined as a vector of Bezier control points
	std::vector<std::vector<Gdiplus::PointF>> mirror_lines_evaluated;
	std::vector<std::vector<Gdiplus::PointF>> type_A_evaluated;
	std::vector<std::vector<Gdiplus::PointF>> type_A_mirror_evaluated;
	std::vector<std::vector<Gdiplus::PointF>> type_B_evaluated;
	std::vector<std::vector<Gdiplus::PointF>> type_B_mirror_evaluated;
	std::vector<std::vector<Gdiplus::PointF>> type_C_evaluated;
	std::vector<std::vector<Gdiplus::PointF>> type_C_mirror_evaluated;
};

