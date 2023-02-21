#include <matrix.h>
#include <gtest/gtest.h>

template<typename T>
T v1(const size_t i, const size_t j)
{
    T i_t = static_cast<T>(i);
    T j_t = static_cast<T>(j);
    return 2*i_t + j_t + 1;
}

template<typename T>
T v2(const size_t i, const size_t j)
{
    T i_t = static_cast<T>(i);
    T j_t = static_cast<T>(j);
    return 2*(1-i_t) + (2 - j_t);
}

TEST(Reduce, Sum)
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    ASSERT_EQ(expr::sum(m), 10);
}

TEST(Reduce, Sum2)
{
    Matrix<int, 2, 2> m1, m2;
    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    m2[0, 0] = 4;
    m2[0, 1] = 3;
    m2[1, 0] = 2;
    m2[1, 1] = 1;

    ASSERT_EQ(expr::sum(m1+m2), 20);
}

TEST(Reduce, Min)
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    ASSERT_EQ(expr::min(m), 1);
}

TEST(Reduce, Max)
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    ASSERT_EQ(expr::max(m), 4);
}

TEST(Reduce, Combined)
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    Matrix<double, 2, 2> res = expr::sum(m)*m;
    for(size_t i = 0; i < res.rows; i++){
        for(size_t j = 0; j < res.cols; j++){
            auto val1 = res[i, j];
            auto val2 = 10*v1<int>(i, j);
            ASSERT_DOUBLE_EQ(val1, val2);
        }
    }
}
