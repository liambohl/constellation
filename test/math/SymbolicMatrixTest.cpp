#include "pch.h"

#include "math/ExpressionFactory.h"
#include "core/Initializer.h"
#include "math/SymbolicMatrix.h"


class SymbolicMatrixTest : public testing::Test {
private:
    Initializer initializer;    // make sure we initialize GDI+
};

TEST_F(SymbolicMatrixTest, translates_points) {
    auto sm = SymbolicMatrix::translate(SymbolicPoint(5.0f, 12.0f));
    auto matrix = sm.evaluate();

    Gdiplus::PointF p1(-5.0f, -12.0f);
    matrix.TransformPoints(&p1);
    EXPECT_FLOAT_EQ(p1.X, 0.0f);
    EXPECT_FLOAT_EQ(p1.Y, 0.0f);

    Gdiplus::PointF p2(2.0f, 14.5f);
    matrix.TransformPoints(&p2);
    EXPECT_FLOAT_EQ(p2.X, 7.0f);
    EXPECT_FLOAT_EQ(p2.Y, 26.5f);
}

TEST_F(SymbolicMatrixTest, translates_points_with_symbols) {
    SymbolicPoint offset("2 * v1 - 0.5 * v2", "v2");
    auto sm = SymbolicMatrix::translate(offset);
    auto matrix1 = sm.evaluate({ {"v1", 5.0f}, {"v2", 4.0f} });     // should have an x-offset of 8 and a y-offset of 4

    Gdiplus::PointF p1(-8.0f, -4.0f);
    matrix1.TransformPoints(&p1);
    EXPECT_FLOAT_EQ(p1.X, 0.0f);
    EXPECT_FLOAT_EQ(p1.Y, 0.0f);

    auto matrix2 = sm.evaluate({ {"v1", 1.0f}, {"v2", -25.0f} });   // should have an x-offset of 14.5 and a y-offset of -25

    Gdiplus::PointF p2(-14.5f, 25.0f);
    matrix2.TransformPoints(&p2);
    EXPECT_FLOAT_EQ(p2.X, 0.0f);
    EXPECT_FLOAT_EQ(p2.Y, 0.0f);
}

TEST_F(SymbolicMatrixTest, rotates_points) {
    // rotate about the origin
    auto sm = SymbolicMatrix::rotate(60.0f);
    auto matrix1 = sm.evaluate();

    Gdiplus::PointF p1(5.0f, -12.0f);
    matrix1.TransformPoints(&p1);
    EXPECT_FLOAT_EQ(p1.X, 12.892305f);
    EXPECT_FLOAT_EQ(p1.Y, -1.669873f);

    // rotate about 4, 1
    SymbolicPoint point(4, 1);
    sm = SymbolicMatrix::rotate(-90.0f, point);
    auto matrix2 = sm.evaluate();

    Gdiplus::PointF p2(-7.0f, 15.0f);
    matrix2.TransformPoints(&p2);
    EXPECT_FLOAT_EQ(p2.X, 18.0f);
    EXPECT_FLOAT_EQ(p2.Y, 12.0f);
}

TEST_F(SymbolicMatrixTest, rotates_points_with_symbols) {
    // rotate about a point defined in terms of v1 and v2
    SymbolicPoint vector_sum("v1_x + v2_x", "v1_y + v2_y");
    auto sm = SymbolicMatrix::rotate(-90.0f, vector_sum);
    // should rotate 90 degrees clockwise around (3, 1)
    auto matrix = sm.evaluate({ {"v1_x", 2.0f}, {"v1_y", 0.0f}, {"v2_x", 1.0f}, {"v2_y", 1.0f} });

    Gdiplus::PointF p1(0.0f, 0.0f);
    matrix.TransformPoints(&p1);
    EXPECT_FLOAT_EQ(p1.X, 2.0f);
    EXPECT_FLOAT_EQ(p1.Y, 4.0f);

    Gdiplus::PointF p2(-12.0f, 5.0f);
    matrix.TransformPoints(&p2);
    EXPECT_FLOAT_EQ(p2.X, 7.0f);
    EXPECT_FLOAT_EQ(p2.Y, 16.0f);
}

TEST_F(SymbolicMatrixTest, reflects_points) {
    auto sm = SymbolicMatrix::reflect(SymbolicLine(1.0f, 0.0f, 0.0f, 1.0f));
    auto matrix1 = sm.evaluate();

    Gdiplus::PointF p1(0.0f, 0.0f);
    matrix1.TransformPoints(&p1);
    EXPECT_FLOAT_EQ(p1.X, 1.0f);
    EXPECT_FLOAT_EQ(p1.Y, 1.0f);

    Gdiplus::PointF p2(-12.0f, 5.0f);
    matrix1.TransformPoints(&p2);
    EXPECT_FLOAT_EQ(p2.X, -4.0f);
    EXPECT_FLOAT_EQ(p2.Y, 13.0f);

    sm = SymbolicMatrix::reflect(SymbolicLine(0.0f, 0.0f, 2.0f, 3.0f));
    auto matrix2 = sm.evaluate();

    Gdiplus::PointF p3(0.0f, 0.0f);
    matrix2.TransformPoints(&p3);
    EXPECT_FLOAT_EQ(p3.X, 0.0f);
    EXPECT_FLOAT_EQ(p3.Y, 0.0f);

    Gdiplus::PointF p4(2.0f, 0.0f);
    matrix2.TransformPoints(&p4);
    EXPECT_FLOAT_EQ(p4.X, -0.769231f);
    EXPECT_FLOAT_EQ(p4.Y, 1.846154f);
}

TEST_F(SymbolicMatrixTest, reflects_points_with_symbols) {
    // the reflection in wallpaper group pm
    auto sm = SymbolicMatrix::reflect(SymbolicLine("v2_x / 2", "v2_y / 2", "v1_x + v2_x / 2", "v1_y + v2_y / 2"));
    // the line of reflection is simply y = 2
    auto matrix = sm.evaluate({ {"v1_x", 10.0f}, {"v1_y", 0.0f}, {"v2_x", 0.0f}, {"v2_y", 4.0f} });

    Gdiplus::PointF p1(0.0f, 0.0f);
    matrix.TransformPoints(&p1);
    EXPECT_FLOAT_EQ(p1.X, 0.0f);
    EXPECT_FLOAT_EQ(p1.Y, 4.0f);

    Gdiplus::PointF p2(5.0f, 1.0f);
    matrix.TransformPoints(&p2);
    EXPECT_FLOAT_EQ(p2.X, 5.0f);
    EXPECT_FLOAT_EQ(p2.Y, 3.0f);
}

TEST_F(SymbolicMatrixTest, applies_transforms_in_sequence) {
    auto rotate = SymbolicMatrix::rotate(90.0f);
    auto translate = SymbolicMatrix::translate(SymbolicPoint(10.0f, 10.0f));

    Gdiplus::Matrix rotate_then_translate = (rotate * translate).evaluate();
    Gdiplus::PointF p1(-12.0f, 5.0f);
    rotate_then_translate.TransformPoints(&p1);
    EXPECT_FLOAT_EQ(p1.X, 5.0f);
    EXPECT_FLOAT_EQ(p1.Y, -2.0f);

    Gdiplus::Matrix translate_then_rotate = (translate * rotate).evaluate();
    Gdiplus::PointF p2(-12.0f, 5.0f);
    translate_then_rotate.TransformPoints(&p2);
    EXPECT_FLOAT_EQ(p2.X, -15.0f);
    EXPECT_FLOAT_EQ(p2.Y, -2.0f);
}

TEST_F(SymbolicMatrixTest, applies_glide_reflections) {
    SymbolicPoint origin = { 0, 0 };
    SymbolicPoint one_zero = { 1, 0 };

    SymbolicMatrix glide_reflection_A = SymbolicMatrix::reflect({ origin, one_zero }) * SymbolicMatrix::translate(one_zero);
    Gdiplus::Matrix glide_reflection_A_concrete = glide_reflection_A.evaluate();

    Gdiplus::PointF p1(-12.0f, 5.0f);
    glide_reflection_A_concrete.TransformPoints(&p1);
    EXPECT_FLOAT_EQ(p1.X, -11.0f);
    EXPECT_FLOAT_EQ(p1.Y, -5.0f);

    SymbolicMatrix glide_reflection_B = SymbolicMatrix::translate(one_zero) * SymbolicMatrix::reflect({ origin, one_zero });
    Gdiplus::Matrix glide_reflection_B_concrete = glide_reflection_B.evaluate();

    Gdiplus::PointF p2(0.25f, 22.5f);
    glide_reflection_B_concrete.TransformPoints(&p2);
    EXPECT_FLOAT_EQ(p2.X, 1.25f);
    EXPECT_FLOAT_EQ(p2.Y, -22.5f);
}
