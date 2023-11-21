#include "DomainBoundaries.h"


DomainBoundaries::DomainBoundaries() {
    // straight line
    mirror_line = {
        { 0.0f, 0.0f },     // point
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f }      // point
    };

    // bump out
    boundary_shape_A = {
        { 0.0f, 0.0f },     // point
        { 0.0f, 0.0f },
        { 0.25f, 0.0f },
        { 0.25f, 0.0f },    // point
        { 0.3f, 0.15f },
        { 0.5f, 0.15f },
        { 0.55f, 0.0f },    // point
        { 0.55f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f }      // point
    };

    // zig zag out
    boundary_shape_B = {
        { 0.0f, 0.0f },     // point
        { 0.0f, 0.0f },
        { 0.45f, 0.0f },
        { 0.45f, 0.0f },    // point
        { 0.45f, 0.0f },
        { 0.6f, 0.15f },
        { 0.6f, 0.15f },    // point
        { 0.6f, 0.15f },
        { 0.75f, 0.0f },
        { 0.75f, 0.0f },    // point
        { 0.75f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f }      // point
    };

    // puzzle piece key
    boundary_shape_C = {
        { 0.0f, 0.0f },     // point
        { 0.0f, 0.0f },
        { 0.3f, 0.0f },
        { 0.3f, 0.0f },     // point
        { 0.3f, 0.0f },
        { 0.33f, 0.01f },
        { 0.33f, 0.04f },   // point
        { 0.33f, 0.07f },
        { 0.3f, 0.08f },
        { 0.3f, 0.11f },    // point
        { 0.3f, 0.14f },
        { 0.33f, 0.17f },
        { 0.39f, 0.17f },   // point
        { 0.45f, 0.17f },
        { 0.48f, 0.14f },
        { 0.48f, 0.11f },   // point
        { 0.48f, 0.08f },
        { 0.45f, 0.07f },
        { 0.45f, 0.04f },   // point
        { 0.45f, 0.01f },
        { 0.48f, 0.0f },
        { 0.48f, 0.0f },    // point
        { 0.48f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 0.0f }      // point
    };
}

void DomainBoundaries::update(std::unordered_map<std::string, float>& variables) {
    mirror_lines_evaluated.clear();
    type_A_evaluated.clear();
    type_A_mirror_evaluated.clear();
    type_B_evaluated.clear();
    type_B_mirror_evaluated.clear();
    type_C_evaluated.clear();
    type_C_mirror_evaluated.clear();

    for (auto& boundary : mirror_lines)
        mirror_lines_evaluated.push_back(evaluate_one(boundary, mirror_line, variables));
    for (auto& boundary : type_A)
        type_A_evaluated.push_back(evaluate_one(boundary, boundary_shape_A, variables));
    for (auto& boundary : type_A_mirror)
        type_A_mirror_evaluated.push_back(evaluate_one(boundary, boundary_shape_A, variables, true));
    for (auto& boundary : type_B)
        type_B_evaluated.push_back(evaluate_one(boundary, boundary_shape_B, variables));
    for (auto& boundary : type_B_mirror)
        type_B_mirror_evaluated.push_back(evaluate_one(boundary, boundary_shape_B, variables, true));
    for (auto& boundary : type_C)
        type_C_evaluated.push_back(evaluate_one(boundary, boundary_shape_C, variables));
    for (auto& boundary : type_C_mirror)
        type_C_mirror_evaluated.push_back(evaluate_one(boundary, boundary_shape_C, variables, true));
}

void DomainBoundaries::draw(Gdiplus::Graphics* graphics, Defaults& defaults) {
    Gdiplus::Pen* pen = defaults.domain_boundary_pen;

    for (auto& control_points : mirror_lines_evaluated)
        graphics->DrawBeziers(pen, control_points.data(), (int)control_points.size());
    for (auto& control_points : type_A_evaluated)
        graphics->DrawBeziers(pen, control_points.data(), (int)control_points.size());
    for (auto& control_points : type_A_mirror_evaluated)
        graphics->DrawBeziers(pen, control_points.data(), (int)control_points.size());
    for (auto& control_points : type_B_evaluated)
        graphics->DrawBeziers(pen, control_points.data(), (int)control_points.size());
    for (auto& control_points : type_B_mirror_evaluated)
        graphics->DrawBeziers(pen, control_points.data(), (int)control_points.size());
    for (auto& control_points : type_C_evaluated)
        graphics->DrawBeziers(pen, control_points.data(), (int)control_points.size());
    for (auto& control_points : type_C_mirror_evaluated)
        graphics->DrawBeziers(pen, control_points.data(), (int)control_points.size());
}

std::vector<Gdiplus::PointF> DomainBoundaries::evaluate_one(
    SymbolicLine& endpoints,
    std::vector<Gdiplus::PointF>& shape,
    std::unordered_map<std::string, float>& variables,
    bool mirrored
) {
    // Get actual endpoints in world space
    float p1_x = endpoints.p1.x->evaluate(variables);
    float p1_y = endpoints.p1.y->evaluate(variables);
    float p2_x = endpoints.p2.x->evaluate(variables);
    float p2_y = endpoints.p2.y->evaluate(variables);

    // transformation from shape space to world space
    // This transform maps (0, 0) to p1 and (1, 0) to p2 with a rotation, scale, and translation
    Gdiplus::Matrix transform(p2_x - p1_x, p2_y - p1_y, p1_y - p2_y, p2_x - p1_x, p1_x, p1_y);

    if (mirrored)
        transform.Scale(1.0f, -1.0f);

    // Actual bezier control points in world space
    std::vector<Gdiplus::PointF> control_points = shape;
    transform.TransformPoints(control_points.data(), (int)control_points.size());
    return control_points;
}

