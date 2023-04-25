#include "transpose_expression.h"
#include <matrix.h>
#include <mdarray.h>
#include <gtest/gtest.h>

TEST(Transpose, FindInSequence)
{
        auto seq = std::index_sequence<1, 2, 3, 4>{};

        ASSERT_EQ((expr::find_in_sequence<0, size_t, 5>(seq)), 4);
        ASSERT_EQ((expr::find_in_sequence<0, size_t, 3>(seq)), 2);
}

TEST(Transpose, TransposeExtents1)
{
        stdex::extents<size_t, 5, 2> exts{};
        auto transposed_exts = expr::reorder_extents(exts, std::index_sequence<1, 0>{});
        ASSERT_EQ(exts.extent(0), transposed_exts.extent(1));
        ASSERT_EQ(transposed_exts.extent(1), 5);
        ASSERT_EQ(exts.extent(1), transposed_exts.extent(0));
        ASSERT_EQ(transposed_exts.extent(0), 2);
}
TEST(Transpose, TransposeExtents2)
{
        stdex::extents<size_t, 5, std::dynamic_extent> exts{2};
        auto transposed_exts = expr::reorder_extents(exts, std::index_sequence<1, 0>{});
        ASSERT_EQ(exts.extent(0), transposed_exts.extent(1));
        ASSERT_EQ(transposed_exts.extent(1), 5);
        ASSERT_EQ(exts.extent(1), transposed_exts.extent(0));
        ASSERT_EQ(transposed_exts.extent(0), 2);
}
TEST(Transpose, TransposeMatrix)
{
        Matrix<int, 2, 3> m1(0);
        m1[0, 2] = 3;
        auto m1_t = expr::transpose(m1, std::index_sequence<1, 0>{});
        ASSERT_EQ(m1.extent(0), m1_t.extent(1));
        ASSERT_EQ(m1.extent(1), m1_t.extent(0));
        for(size_t i = 0; i < 2; i++){
                for(size_t j = 0; j < 3; j++){
                        auto v1 = m1[i, j], v2 = m1_t[j, i];
                        ASSERT_EQ(v1, v2);
                }
        }
}
