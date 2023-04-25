#include "matrix_multiplication_expression.h"
#include <matrix.h>
#include <mdarray.h>
#include <gtest/gtest.h>

TEST(Matmul, CompileTimeExtents23)
{
        stdex::extents<size_t, 2, 5> ex1{};
        stdex::extents<size_t, 5, 3> ex2{};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex2.extent(1));
}
TEST(Matmul, RunTimeExtents23)
{
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent> ex1{2, 5};
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent> ex2{5, 3};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex2.extent(1));
}
TEST(Matmul, CompileTimeRunTimeExtents23)
{
        stdex::extents<size_t, 2, 5> ex1{};
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent> ex2{5, 3};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex2.extent(1));
}
TEST(Matmul, RunTimeCompileTimeExtents23)
{
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent> ex1{2, 5};
        stdex::extents<size_t, 5, 3> ex2{};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex2.extent(1));
}
TEST(Matmul, CompileTimeExtents123)
{
        stdex::extents<size_t, 1, 2, 5> ex1{};
        stdex::extents<size_t, 1, 5, 3> ex2{};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(0), ex2.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex1.extent(1));
        ASSERT_EQ(mm_ext.extent(2), ex2.extent(2));
}
TEST(Matmul, RunTimeExtents123)
{
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> ex1{1, 2, 5};
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> ex2{1, 5, 3};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(0), ex2.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex1.extent(1));
        ASSERT_EQ(mm_ext.extent(2), ex2.extent(2));
}
TEST(Matmul, CompileTimeRunTimeExtents123)
{
        stdex::extents<size_t, 1, 2, 5> ex1{};
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> ex2{1, 5, 3};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(0), ex2.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex1.extent(1));
        ASSERT_EQ(mm_ext.extent(2), ex2.extent(2));
}
TEST(Matmul, RunTimeCompileTimeExtents123)
{
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> ex1{1, 2, 5};
        stdex::extents<size_t, 1, 5, 3> ex2{};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(0), ex2.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex1.extent(1));
        ASSERT_EQ(mm_ext.extent(2), ex2.extent(2));
}
TEST(Matmul, CompileTimeExtents4123)
{
        stdex::extents<size_t, 4, 1, 2, 5> ex1{};
        stdex::extents<size_t, 4, 1, 5, 3> ex2{};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(0), ex2.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex1.extent(1));
        ASSERT_EQ(mm_ext.extent(1), ex2.extent(1));
        ASSERT_EQ(mm_ext.extent(2), ex1.extent(2));
        ASSERT_EQ(mm_ext.extent(3), ex2.extent(3));
}
TEST(Matmul, RunTimeExtents4123)
{
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> ex1{4, 1, 2, 5};
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> ex2{4, 1, 5, 3};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(0), ex2.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex1.extent(1));
        ASSERT_EQ(mm_ext.extent(1), ex2.extent(1));
        ASSERT_EQ(mm_ext.extent(2), ex1.extent(2));
        ASSERT_EQ(mm_ext.extent(3), ex2.extent(3));
}
TEST(Matmul, CompileTimeRunTimeExtents4123)
{
        stdex::extents<size_t, 4, 1, 2, 5> ex1{};
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> ex2{4, 1, 5, 3};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(0), ex2.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex1.extent(1));
        ASSERT_EQ(mm_ext.extent(1), ex2.extent(1));
        ASSERT_EQ(mm_ext.extent(2), ex1.extent(2));
        ASSERT_EQ(mm_ext.extent(3), ex2.extent(3));
}
TEST(Matmul, RunTimeCompileTimeExtents4123)
{
        stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent, std::dynamic_extent> ex1{4, 1, 2, 5};
        stdex::extents<size_t, 4, 1, 5, 3> ex2{};
        auto mm_ext = expr::build_matmul_extents(ex1, ex2);
        ASSERT_EQ(mm_ext.extent(0), ex1.extent(0));
        ASSERT_EQ(mm_ext.extent(0), ex2.extent(0));
        ASSERT_EQ(mm_ext.extent(1), ex1.extent(1));
        ASSERT_EQ(mm_ext.extent(1), ex2.extent(1));
        ASSERT_EQ(mm_ext.extent(2), ex1.extent(2));
        ASSERT_EQ(mm_ext.extent(3), ex2.extent(3));
}

TEST(Matmul, CompileTimeExtentsMatMul22)
{
        Matrix<int, 2, 4> m1(0);
        Matrix<int, 4, 2> m2(0);
        m1[0,0] = 1;
        m1[0,1] = 0;
        m1[1,0] = 0;
        m1[1,1] = 1;
        m2[0,0] = 0;
        m2[0,1] = 1;
        m2[1,0] = 1;
        m2[1,1] = 0;
        auto mm = expr::matmul(m1, m2);
        ASSERT_EQ((mm[0, 0]), 0);
        ASSERT_EQ((mm[0, 1]), 1);
        ASSERT_EQ((mm[1, 0]), 1);
        ASSERT_EQ((mm[1, 1]), 0);
}
TEST(Matmul, RunTimeCompileTimeExtentsMatMul22)
{
        using D2 = stdex::dextents<std::size_t, 2>;
        D2 ex1{2, 4};
        MDArray<int, D2> m1(ex1, 0);
        Matrix<int, 4, 2> m2(0);
        m1[0,0] = 1;
        m1[0,1] = 0;
        m1[1,0] = 0;
        m1[1,1] = 1;
        m2[0,0] = 0;
        m2[0,1] = 1;
        m2[1,0] = 1;
        m2[1,1] = 0;
        auto mm = expr::matmul(m1, m2);
        ASSERT_EQ((mm[0, 0]), 0);
        ASSERT_EQ((mm[0, 1]), 1);
        ASSERT_EQ((mm[1, 0]), 1);
        ASSERT_EQ((mm[1, 1]), 0);
}
TEST(Matmul, CompileTimeRunTimeExtentsMatMul22)
{
        using D2 = stdex::dextents<std::size_t, 2>;
        Matrix<int, 2, 4> m1(0);
        D2 ex2{4, 2};
        MDArray<int, D2> m2(ex2, 0);
        m1[0,0] = 1;
        m1[0,1] = 0;
        m1[1,0] = 0;
        m1[1,1] = 1;
        m2[0,0] = 0;
        m2[0,1] = 1;
        m2[1,0] = 1;
        m2[1,1] = 0;
        auto mm = expr::matmul(m1, m2);
        ASSERT_EQ((mm[0, 0]), 0);
        ASSERT_EQ((mm[0, 1]), 1);
        ASSERT_EQ((mm[1, 0]), 1);
        ASSERT_EQ((mm[1, 1]), 0);
}
TEST(Matmul, RunTimeRunTimeExtentsMatMul22)
{
        using D2 = stdex::dextents<std::size_t, 2>;
        D2 ex1{2, 4};
        D2 ex2{4, 2};
        MDArray<int, D2> m1(ex1, 0), m2(ex2, 0);
        m1[0,0] = 1;
        m1[0,1] = 0;
        m1[1,0] = 0;
        m1[1,1] = 1;
        m2[0,0] = 0;
        m2[0,1] = 1;
        m2[1,0] = 1;
        m2[1,1] = 0;
        auto mm = expr::matmul(m1, m2);
        ASSERT_EQ((mm[0, 0]), 0);
        ASSERT_EQ((mm[0, 1]), 1);
        ASSERT_EQ((mm[1, 0]), 1);
        ASSERT_EQ((mm[1, 1]), 0);
}
TEST(Matmul, CompileTimeExtentsMatMul4122)
{
        using C4L = stdex::extents<size_t, 4, 1, 2, 5>;
        using C4R = stdex::extents<size_t, 4, 1, 5, 2>;
        MDArray<int, C4L> m1(C4L(), 0);
        MDArray<int, C4R> m2(C4R(), 0);
        m1[0, 0, 0, 0] = 1;
        m1[0, 0, 0, 1] = 0;
        m1[0, 0, 1, 0] = 0;
        m1[0, 0, 1, 1] = 1;
        m2[0, 0, 0, 0] = 0;
        m2[0, 0, 0, 1] = 1;
        m2[0, 0, 1, 0] = 1;
        m2[0, 0, 1, 1] = 0;
        auto mm = expr::matmul(m1, m2);
        ASSERT_EQ((mm[0, 0, 0, 0]), 0);
        ASSERT_EQ((mm[0, 0, 0, 1]), 1);
        ASSERT_EQ((mm[0, 0, 1, 0]), 1);
        ASSERT_EQ((mm[0, 0, 1, 1]), 0);
}
TEST(Matmul, RunTimeCompileTimeExtentsMatMul4122)
{
        using D4L = stdex::dextents<std::size_t, 4>;
        using C4R = stdex::extents<std::size_t, 4, 1, 5, 2>;
        D4L ex1{4, 1, 2, 5};
        MDArray<int, D4L> m1(ex1, 0);
        MDArray<int, C4R> m2(C4R(), 0);
        m1[0, 0, 0, 0] = 1;
        m1[0, 0, 0, 1] = 0;
        m1[0, 0, 1, 0] = 0;
        m1[0, 0, 1, 1] = 1;
        m2[0, 0, 0, 0] = 0;
        m2[0, 0, 0, 1] = 1;
        m2[0, 0, 1, 0] = 1;
        m2[0, 0, 1, 1] = 0;
        auto mm = expr::matmul(m1, m2);
        ASSERT_EQ((mm[0, 0, 0, 0]), 0);
        ASSERT_EQ((mm[0, 0, 0, 1]), 1);
        ASSERT_EQ((mm[0, 0, 1, 0]), 1);
        ASSERT_EQ((mm[0, 0, 1, 1]), 0);
}
TEST(Matmul, CompileTimeRunTimeExtentsMatMul4122)
{
        using D4R = stdex::dextents<std::size_t, 4>;
        using C4L = stdex::extents<std::size_t, 4, 1, 2, 5>;
        MDArray<int, C4L> m1(C4L(), 0);
        D4R ex2{4, 1, 5, 2};
        MDArray<int, D4R> m2(ex2, 0);
        m1[0, 0, 0, 0] = 1;
        m1[0, 0, 0, 1] = 0;
        m1[0, 0, 1, 0] = 0;
        m1[0, 0, 1, 1] = 1;
        m2[0, 0, 0, 0] = 0;
        m2[0, 0, 0, 1] = 1;
        m2[0, 0, 1, 0] = 1;
        m2[0, 0, 1, 1] = 0;
        auto mm = expr::matmul(m1, m2);
        ASSERT_EQ((mm[0, 0, 0, 0]), 0);
        ASSERT_EQ((mm[0, 0, 0, 1]), 1);
        ASSERT_EQ((mm[0, 0, 1, 0]), 1);
        ASSERT_EQ((mm[0, 0, 1, 1]), 0);
}
TEST(Matmul, RunTimeRunTimeExtentsMatMul4122)
{
        using D4 = stdex::dextents<std::size_t, 4>;
        D4 ex1{4, 1, 2, 5};
        D4 ex2{4, 1, 5, 2};
        MDArray<int, D4> m1(ex1, 0), m2(ex2, 0);
        m1[0, 0, 0, 0] = 1;
        m1[0, 0, 0, 1] = 0;
        m1[0, 0, 1, 0] = 0;
        m1[0, 0, 1, 1] = 1;
        m2[0, 0, 0, 0] = 0;
        m2[0, 0, 0, 1] = 1;
        m2[0, 0, 1, 0] = 1;
        m2[0, 0, 1, 1] = 0;
        auto mm = expr::matmul(m1, m2);
        ASSERT_EQ((mm[0, 0, 0, 0]), 0);
        ASSERT_EQ((mm[0, 0, 0, 1]), 1);
        ASSERT_EQ((mm[0, 0, 1, 0]), 1);
        ASSERT_EQ((mm[0, 0, 1, 1]), 0);
}
